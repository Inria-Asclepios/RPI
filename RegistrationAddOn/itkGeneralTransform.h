/*=========================================================================
 
 Program:   vtkINRIA3D
 Module:    $Id: itkGeneralTransform.h 1 2008-01-22 19:01:33Z ntoussaint $
 Language:  C++
 Author:    $Author: ntoussaint $
 Date:      $Date: 2008-01-22 20:01:33 +0100 (Tue, 22 Jan 2008) $
 Version:   $Revision: 1 $
 
 Copyright (c) 2007 INRIA - Asclepios Project. All rights reserved.
 See Copyright.txt for details.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.
 
 =========================================================================*/
#ifndef _itk_GeneralTransform_h_
#define _itk_GeneralTransform_h_

#include "itkTransform.h"
#include "itkProcessObject.h"

#include "itkMatrixOffsetTransformBase.h"

#include <vector>


namespace itk
{
    /**
     \class GeneralTransform (itk)
     
     \brief Plug transformations into a pipeline.
     This class was contributed by Nicolas Toussaint, INRIA.
     This class provides API for a transformation pipeline.
     
     Inputs are some transforms of any type (i.e. affine, rigid, similarity,
     displacement-field based, etc...) that the user can insert with InsertNextTransform().
     
     Input transforms are stored into a temporary container and marked as <<available>>.
     Calling Push() after having inserted a new transform will plug it at the end of the
     current pipeline. Calling Pull() will unplug the last <<pushed>> transform. This allow
     the user to go back and forth in the transformation pipeline.
     
     Use RemoveAllTransforms() or SetIdentity() for convinience.
     
     An input transform can be set, it will always be applied at the beginning of the pipeline.
     
     If all the transforms of the pipeline (including the input) are linear based (e.g. Rigid, Similarity,
     or affine) the user can export the global transformation description
     as a MatrixOffsetTransformBase class, for that use GetGlobalLinearTransform().
     
     \ingroup   ProcessObject
     \author Nicolas Toussaint, Florence Dru, INRIA
     
     */
    template <class TScalarType=float, unsigned int NDimensions=3>
    class ITK_EXPORT GeneralTransform : public Transform<TScalarType, NDimensions, NDimensions>
    {
        
    public:
        
        /** general and pointer type */
        typedef GeneralTransform         Self;
        typedef Transform<TScalarType, NDimensions, NDimensions> Superclass;
        typedef SmartPointer<Self>       Pointer;
        typedef SmartPointer<const Self> ConstPointer;
        /** Dimension of the domain space. */
        itkStaticConstMacro(SpaceDimension, unsigned int, NDimensions);
        itkStaticConstMacro(ParametersDimension, unsigned int, NDimensions);
        /** generic constructors */
        itkNewMacro (Self);
        itkTypeMacro (GeneralTransform, Transform);
        /**
         Transform type: This class pipelines transforms of type
         TransformType or subclasses together
         */
        typedef Superclass TransformType;
        typedef typename TransformType::Pointer TransformPointerType;
        typedef typename TransformType::ConstPointer TransformConstPointerType;
        /** Type of the scalar representing coordinate and vector elements. */
        typedef TScalarType ScalarType;
        /** Type of the input parameters. */
        typedef typename Superclass::ParametersType ParametersType;
        /** Type of the Jacobian matrix. */
        typedef typename Superclass::JacobianType JacobianType;
        /** Standard vector type for this class. */
        typedef typename Superclass::InputVectorType InputVectorType;
        typedef typename Superclass::OutputVectorType OutputVectorType;
        /** Standard covariant vector type for this class */
        typedef typename Superclass::InputCovariantVectorType InputCovariantVectorType;
        typedef typename Superclass::OutputCovariantVectorType OutputCovariantVectorType;
        /** Standard vnl_vector type for this class. */
        typedef typename Superclass::InputVnlVectorType InputVnlVectorType;
        typedef typename Superclass::OutputVnlVectorType OutputVnlVectorType;
        /** Matrix type. */
        typedef itk::Matrix<double, NDimensions+1, NDimensions+1>  OutputMatrixType; //homogemous matrix
        
        /** Standard coordinate point type for this class */
        typedef typename Superclass::InputPointType InputPointType;
        typedef typename Superclass::OutputPointType OutputPointType;
        /** base transform type for linear+offset transformations */
        typedef itk::MatrixOffsetTransformBase<ScalarType, NDimensions, NDimensions> MatrixOffsetTransformType;
        typedef typename MatrixOffsetTransformType::Pointer MatrixOffsetTransformPointerType;
        typedef typename MatrixOffsetTransformType::ConstPointer MatrixOffsetTransformConstPointerType;
        /** type of transform lists */
        typedef  std::vector<TransformConstPointerType> TransformListType;
        
        
        
        /**  Method to transform a point. */
        virtual OutputPointType TransformPoint(const InputPointType  & ) const ITK_OVERRIDE;
        /**  Method to transform a vector. */
        virtual OutputVectorType    TransformVector(const InputVectorType &) const ITK_OVERRIDE;
        /**  Method to transform a vnl_vector. */
        virtual OutputVnlVectorType TransformVector(const InputVnlVectorType &) const ITK_OVERRIDE;
        /**  Method to transform a CovariantVector. */
        virtual OutputCovariantVectorType TransformCovariantVector(const InputCovariantVectorType &) const ITK_OVERRIDE;
        
        virtual void SetFixedParameters(const ParametersType &) ITK_OVERRIDE;
        
        /**
         Set the transformation parameters and update internal transformation.
         * SetParameters gives the transform the option to set it's
         * parameters by keeping a reference to the parameters, or by
         * copying.  To force the transform to copy it's parameters call
         * SetParametersByValue.
         * \sa SetParametersByValue
         *
         * Not Supported
         */
        virtual void SetParameters(const ParametersType &) ITK_OVERRIDE;
        
        /**
         Set the transformation parameters and update internal transformation.
         * This method forces the transform to copy the parameters.  The
         * default implementation is to call SetParameters.  This call must
         * be overridden if the transform normally implements SetParameters
         * by keeping a reference to the parameters.
         * \sa SetParameters
         *
         * Not Supported
         
         */
        virtual void SetParametersByValue (const ParametersType & p) ITK_OVERRIDE
        { (void) p; itkExceptionMacro("GeneralTransform<TScalarType, NDimensions> does not handle any parameter system yet"); }
        
        /**
         * Get the Transformation Parameters.
         *
         * Not Supported
         */
        virtual const ParametersType& GetParameters() const ITK_OVERRIDE;
        
        /**
         Get the inverse of this transform
         
         Not supported
         */
        virtual bool GetInverse( Self* inverse) const;
        /**
         Remove all transforms from pipeline and remove the input.
         Careful : you will not be able to recover transforms from the pipeline
         after calling SetIdentity()
         */
        virtual void SetIdentity(void);
        
        virtual MatrixOffsetTransformPointerType GetGlobalLinearTransform (void) const;
        /**
         Get the number of transform currently in stack
         */
        unsigned int GetNumberOfTransformsInStack (void) const
        { return this->m_TransformList.size(); }
        /**
         Indicates if the transforms is linear or not.
         
         A transform is linear if,
         given two points P and Q, and scalar coefficients a and b, then
         
         T( a*P + b*Q ) = a * T(P) + b * T(Q)
         
         The composition (pipeline) of transforms is linear if and only if all
         the inside transforms are linear, this method calls iteratively IsLinear().
         
         Hence the return value can be different after or befor a call of Push().
         */
        virtual bool IsLinear(void) const ITK_OVERRIDE;
        
        bool Undo(void);
        bool Redo(void);
        
        unsigned int InsertTransform(TransformConstPointerType arg);
        TransformConstPointerType GetTransform(unsigned int i);
        void RemoveAllTransforms(void);
        
        // Purposedly not implemented
        void ComputeJacobianWithRespectToParameters(const InputPointType  & itkNotUsed(p),
                                                    JacobianType & itkNotUsed(jacobian) ) const ITK_OVERRIDE
        {}

    protected:
        
        /** Print contents of an TranslationTransform. */
        void PrintSelf(std::ostream &os, Indent indent) const ITK_OVERRIDE;
        
        unsigned int RemoveTransform(TransformConstPointerType arg);
        
        GeneralTransform();
        virtual ~GeneralTransform();
        
        TransformListType m_RemovedTransformList;
        TransformListType m_TransformList;
    };  
} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGeneralTransform.txx"
#endif

#endif
