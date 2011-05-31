#ifndef _itkStationaryVelocityFieldTransform_h_
#define _itkStationaryVelocityFieldTransform_h_

#include <itkTransform.h>
#include <itkProcessObject.h>
#include <itkConstNeighborhoodIterator.h>
#include <itkVectorLinearInterpolateNearestNeighborExtrapolateImageFunction.h>
#include <itkImage.h>


namespace itk
{

 /**
  * @description StationaryVelocityFieldTransform (itk)
  */

template <class TScalarType=float, unsigned int NDimensions=3>
class ITK_EXPORT StationaryVelocityFieldTransform : public Transform<TScalarType, NDimensions, NDimensions>
{

public:

    typedef  StationaryVelocityFieldTransform                  Self;
    typedef  Transform<TScalarType, NDimensions, NDimensions>  Superclass;
    typedef  SmartPointer<Self>                                Pointer;
    typedef  SmartPointer<const Self>                          ConstPointer;

    /** Dimension of the domain space. */
    itkStaticConstMacro( SpaceDimension,      unsigned int, NDimensions);
    itkStaticConstMacro( ParametersDimension, unsigned int, NDimensions);

    /** Generic constructors */
    itkNewMacro(Self);
    itkTypeMacro (StationaryVelocityFieldTransform, Transform);

    typedef Superclass TransformType;
    typedef typename TransformType::ConstPointer TransformPointerType;

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
    typedef typename Superclass::InputCovariantVectorType  InputCovariantVectorType;
    typedef typename Superclass::OutputCovariantVectorType OutputCovariantVectorType;

    /** Standard vnl_vector type for this class. */
    typedef typename Superclass::InputVnlVectorType  InputVnlVectorType;
    typedef typename Superclass::OutputVnlVectorType OutputVnlVectorType;

    /** Standard coordinate point type for this class */
    typedef typename Superclass::InputPointType  InputPointType;
    typedef typename Superclass::OutputPointType OutputPointType;

    typedef itk::Vector<ScalarType, NDimensions>      VectorType;
    typedef itk::Image<VectorType, NDimensions>       VelocityFieldType;
    typedef typename VelocityFieldType::Pointer       VelocityFieldPointerType;
    typedef typename VelocityFieldType::ConstPointer  VelocityFieldConstPointerType;
    typedef typename VelocityFieldType::IndexType     VelocityFieldIndexType;

    /** Type relative to the field geometry */
    typedef typename VelocityFieldType::PointType      OriginType;
    typedef typename VelocityFieldType::SpacingType    SpacingType;
    typedef typename VelocityFieldType::DirectionType  DirectionType;
    typedef typename VelocityFieldType::RegionType     RegionType;

    typedef itk::VectorLinearInterpolateNearestNeighborExtrapolateImageFunction<VelocityFieldType, ScalarType> InterpolateFunctionType;
    typedef typename InterpolateFunctionType::Pointer InterpolateFunctionPointerType;

    /**
       Type of the iterator that will be used to move through the image.  Also
	the type which will be passed to the evaluate function
    */
    typedef ConstNeighborhoodIterator<VelocityFieldType> ConstNeighborhoodIteratorType;
    typedef typename ConstNeighborhoodIteratorType::RadiusType RadiusType;

    /**  Method to transform a point. */
    virtual OutputPointType TransformPoint(const InputPointType  & ) const;
    /**  Method to transform a vector. */
    virtual OutputVectorType    TransformVector(const InputVectorType &) const;
    /**  Method to transform a vnl_vector. */
    virtual OutputVnlVectorType TransformVector(const InputVnlVectorType &) const;
    /**  Method to transform a CovariantVector. */
    virtual OutputCovariantVectorType TransformCovariantVector(const InputCovariantVectorType &) const;
    /**
       Set the transformation parameters and update internal transformation.
       * SetParameters gives the transform the option to set it's
       * parameters by keeping a reference to the parameters, or by
       * copying.  To force the transform to copy it's parameters call
       * SetParametersByValue.
       * \sa SetParametersByValue
       * 
       ######## NOT SUPPORTED ############
       */
    virtual void SetParameters( const ParametersType & )
    {
        itkExceptionMacro ("this type of transform does not handle any parameters yet !");
    };
    /**
       Set the transformation parameters and update internal transformation. 
       * This method forces the transform to copy the parameters.  The
       * default implementation is to call SetParameters.  This call must
       * be overridden if the transform normally implements SetParameters
       * by keeping a reference to the parameters.
       * \sa SetParameters

       ######## NOT SUPPORTED ############
       
    */
    virtual void SetParametersByValue ( const ParametersType & p )
    {
        itkExceptionMacro ("this type of transform does not handle any parameters yet !");
    };

    /**
       Get the Transformation Parameters.
       
       ######## NOT SUPPORTED ############ 
    */
    virtual const ParametersType& GetParameters(void) const
    {
        itkExceptionMacro ("this type of transform does not handle any parameters yet !");
    }


    virtual const JacobianType & GetJacobian(const InputPointType  &) const
    {
        itkExceptionMacro ("this type of transform does not handle Jacobian !");
    }
    /**
       Get the jacobian of the transformation with respect to the coordinates at a specific point
    */
    virtual vnl_matrix_fixed<double,NDimensions,NDimensions> GetJacobianWithRespectToCoordinates(const InputPointType  &) const;
    /**
       Get the jacobian determinant of transformation with respect to the coordinates at a specific point
    */
    virtual ScalarType GetJacobianDeterminantWithRespectToCoordinates(const InputPointType  &) const;
    /**
       Get the inverse of this transform

       ######## NOT SUPPORTED ############ 
    */
    virtual bool GetInverse( Self* inverse) const
    {
        itkExceptionMacro ("this type of transform does not handle Inversion !");
        return false;
    }

    /**
       Set the transform to identity. Basically remove (and unregister)
       the velocity field from the transform
    */
    virtual void SetIdentity(void)
    {
        m_VelocityField = NULL;
    }
    /**
       Set/Get the velocity field used for transforming points and vectors
    */
    itkGetConstObjectMacro( VelocityField, VelocityFieldType );
    virtual void SetVelocityField( VelocityFieldConstPointerType field);


    /**
     * Gets the origin of the field.
     */
    OriginType GetOrigin(void)
    {
        return m_VelocityField->GetOrigin();
    }


    /**
     * Gets the spacing of the field.
     */
    SpacingType GetSpacing(void)
    {
        return m_VelocityField->GetSpacing();
    }


    /**
     * Gets the direction of the field.
     */
    DirectionType GetDirection(void)
    {
        return m_VelocityField->GetDirection();
    }


    /**
     * Gets the region object that defines the size and starting index for the largest possible
     * region this image could represent.
     */
    RegionType GetLargestPossibleRegion(void)
    {
        return m_VelocityField->GetLargestPossibleRegion();
    }


protected:

    /** Print contents of an TranslationTransform. */
    void PrintSelf(std::ostream &os, Indent indent) const;

    StationaryVelocityFieldTransform();
    virtual ~StationaryVelocityFieldTransform() {};

    VelocityFieldConstPointerType m_VelocityField;
    InterpolateFunctionPointerType m_InterpolateFunction;

private:

    /** The weights used to scale partial derivatives during processing */
    double m_DerivativeWeights[NDimensions];

};


} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkStationaryVelocityFieldTransform.txx"
#endif


#endif
