//
//  ShapeFunctionHexTrilinear.h
//  Gauss
//
//  Created by David Levin on 4/27/17.
//
//

#ifndef ShapeFunctionHexTrilinear_h
#define ShapeFunctionHexTrilinear_h

namespace Gauss {
    namespace FEM {

        //TODO Add trilinear hexahedral shape function + approriate quadrature rules
        template<typename DataType>
        class ShapeFunctionHexTrilinear {
        public:
            
            template<typename QDOFList, typename QDotDOFList>
            ShapeFunctionHexTrilinear(Eigen::MatrixXd &V, Eigen::MatrixXi &F, QDOFList &qDOFList, QDotDOFList &qDotDOFList) {

                //for the time being assume things come as a stack (qdofs and qdotdofs)
                m_qDofs[0] = qDOFList[0];
                m_qDofs[1] = qDOFList[1];
                m_qDofs[2] = qDOFList[2];
                m_qDofs[3] = qDOFList[3];
                m_qDofs[4] = qDOFList[4];
                m_qDofs[5] = qDOFList[5];
                m_qDofs[6] = qDOFList[6];
                m_qDofs[7] = qDOFList[7];
                
                
                m_qDotDofs[0] = qDotDOFList[0];
                m_qDotDofs[1] = qDotDOFList[1];
                m_qDotDofs[2] = qDotDOFList[2];
                m_qDotDofs[3] = qDotDOFList[3];
                m_qDotDofs[4] = qDotDOFList[4];
                m_qDotDofs[5] = qDotDOFList[5];
                m_qDotDofs[6] = qDotDOFList[6];
                m_qDotDofs[7] = qDotDOFList[7];
                
                m_x0 << Vert(0,0), Vert(0,1), Vert(0,2);
                m_dx = V.block(F(6), 0, 1,3) - V.block(F(0), 0, 1, 3);
                
                //using node numberings from http://www.colorado.edu/engineering/CAS/courses.d/AFEM.d/AFEM.Ch11.d/AFEM.Ch11.pdf
                //but with right handed coordinate system
                //     4----------5
                //    /|         /|
                //   7-|--------6 |
                //   | |        | |
                //   | 0 -------|-1    +y
                //   |/         |/     |
                //   3----------2      0-- +x
                //                    /
                //                   +z
                
                double x[3];
                phi<5>(&x[0]);
                
            }
            
            //drop my gets for this just because my hands get tired of typing it all the time
            template<unsigned int Vertex>
            inline double phi(double *x) {
                
                assert(Vertex < 8);
                Eigen::Vector3d e = alpha(x);
                
                static_if<(Vertex==0)>([&](auto f) {
                    return (1.0/8.0)*(1-e(0))*(1-e(1))*(1-e(2));
                }).else_([&](auto f) {
                    static_if<(Vertex==1)>([&](auto f) {
                        return (1.0/8.0)*(1+e(0))*(1-e(1))*(1-e(2));
                    }).else_([&](auto f) {
                        static_if<(Vertex==2)>([&](auto f) {
                            return (1.0/8.0)*(1+e(0))*(1+e(1))*(1-e(2));
                        }).else_([&](auto f) {
                            static_if<(Vertex==3)>([&](auto f) {
                                return (1.0/8.0)*(1-e(0))*(1+e(1))*(1-e(2));
                            }).else_([&](auto f) {
                                static_if<(Vertex==4)>([&](auto f) {
                                    return (1.0/8.0)*(1-e(0))*(1-e(1))*(1+e(2));
                                }).else_([&](auto f) {
                                    static_if<(Vertex==5)>([&](auto f) {
                                        return (1.0/8.0)*(1+e(0))*(1-e(1))*(1+e(2));
                                    }).else_([&](auto f) {
                                        static_if<(Vertex==6)>([&](auto f) {
                                            return (1.0/8.0)*(1+e(0))*(1+e(1))*(1+e(2));
                                        }).else_([&](auto f) {
                                            //final shape function
                                            return (1.0/8.0)*(1-e(0))*(1+e(1))*(1+e(2));
                                        });
                                    });
                                });
                            });
                        });
                    });
                    
                });
                
                assert(1==0);
                std::cout<<"Error, should not reach here \n";
                return 0.0;
            }
            
            inline Eigen::Vector3x<DataType> x(double alphaX, double alphaY, double alphaZ) const {
                return m_x0 + Eigen::Vector3x<DataType>((alphaX+1.)*m_dx(0)*0.5, (alphaY+1.)*m_dx(1)*0.5,(alphaZ+1.)*m_dx(2)*0.5);
            }
            
            inline Eigen::Vector3d alpha(double *x) {
            
                Eigen::Vector3d e = Eigen::Map<Eigen::Vector3d>(x)-m_x0;
                e(0) = 2.0*(e(0)/m_dx(0))-1.0;
                e(1) = 2.0*(e(1)/m_dx(1))-1.0;
                e(2) = 2.0*(e(2)/m_dx(2))-1.0;
                
                return e;
            }
            
            template<unsigned int Vertex>
            inline std::array<DataType, 3> dphi(double *x) {
                
            }
            
            
            //Kinematics stuff (break into new template class then it suits me
            template<typename Matrix>
            inline void F(Matrix &output, State<DataType> &state) {
                
                std::cout<<"Not implemented yet \n";
                assert(1 == 0);
            }
            
            //Jacobian: derivative with respect to degrees of freedom
            template<typename Matrix>
            inline void J(Matrix &output, double *x, State<DataType> &state) {
                
                //just a 3x12 matrix of shape functions
                //kind of assuming everything is initialized before we get here
                double phi0 = phi<0>(x);
                double phi1 = phi<1>(x);
                double phi2 = phi<2>(x);
                double phi3 = phi<3>(x);
                double phi4 = phi<4>(x);
                double phi5 = phi<5>(x);
                double phi6 = phi<6>(x);
                double phi7 = phi<7>(x);

                output.resize(3,24);
                output.setZero();
                output.block(0,0, 3,3) = phi0*Matrix::Identity();
                output.block(0,3, 3,3) = phi1*Matrix::Identity();
                output.block(0,6, 3,3) = phi2*Matrix::Identity();
                output.block(0,9, 3,3) = phi3*Matrix::Identity();
                output.block(0,12, 3,3) = phi4*Matrix::Identity();
                output.block(0,15, 3,3) = phi5*Matrix::Identity();
                output.block(0,18, 3,3) = phi6*Matrix::Identity();
                output.block(0,21, 3,3) = phi7*Matrix::Identity();
                
            }
            
            
            constexpr unsigned int getNumVerts() { return 8; }
            
        protected:
            
            std::array<DOFBase<DataType,0> *, 8> m_qDofs;
            std::array<DOFBase<DataType,1> *, 8> m_qDotDofs;
            Eigen::Vector3d m_dx;
            Eigen::Vector3d m_x0;
            
        private:
            
        };
    }
}

#endif /* ShapeFunctionHexTrilinear_h */
