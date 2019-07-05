#ifndef _RPI_DIFFEOMORPHIC_DEMONS_CXX_
#define _RPI_DIFFEOMORPHIC_DEMONS_CXX_



#include <itkMultiResolutionPDEDeformableRegistration.h>
#include <itkHistogramMatchingImageFilter.h>
#include <itkDiffeomorphicDemonsRegistrationFilter.h>
#include <itkFastSymmetricForcesDemonsRegistrationFilter.h>
#include "rpiDiffeomorphicDemons.hxx"



// Namespace RPI : Registration Programming Interface
namespace rpi
{



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::DiffeomorphicDemons(void)
{

    // Initialize parameters (except iterations)
    this->m_updateRule                         = UPDATE_DIFFEOMORPHIC;
    this->m_gradientType                       = GRADIENT_SYMMETRIZED;
    this->m_maximumUpdateStepLength            = 2.0;
    this->m_updateFieldStandardDeviation       = 0.0;
    this->m_displacementFieldStandardDeviation = 1.5;

    // Initialize iterations
    this->m_iterations.resize(3);
    this->m_iterations[0] = 15;
    this->m_iterations[1] = 10;
    this->m_iterations[2] =  5;

    // Initialize the transformation
    this->m_transform = TransformType::New();
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::~DiffeomorphicDemons(void)
{
    // Do nothing
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
std::vector<unsigned int>
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetNumberOfIterations(void) const
{
    return this->m_iterations;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::SetNumberOfIterations(std::vector<unsigned int> iterations)
{
    this->m_iterations = iterations;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
typename DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >::UpdateRule
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetUpdateRule(void) const
{
    return this->m_updateRule;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
std::string
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetUpdateRuleAsString(void) const
{
    std::string type;
    if ( this->m_updateRule == UPDATE_DIFFEOMORPHIC )
        return ( type = "diffeomorphic" );
    else if ( this->m_updateRule == UPDATE_ADDITIVE )
        return ( type = "additive" );
    else // m_updateRule == UPDATE_COMPOSITIVE
        return ( type = "compositive" );
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::SetUpdateRule(UpdateRule value)
{
    this->m_updateRule = value;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
typename DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >::GradientType
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetGradientType(void) const
{
    return this->m_gradientType;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
std::string
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetGradientTypeAsString(void) const
{
    std::string type;
    if ( this->m_gradientType == GRADIENT_SYMMETRIZED )
        return ( type = "symmetrized" );
    else if ( this->m_gradientType == GRADIENT_FIXED_IMAGE )
        return ( type = "fixed image" );
    else if ( this->m_gradientType == GRADIENT_WARPED_MOVING_IMAGE )
        return ( type = "warped moving image" );
    else // m_gradientType == GRADIENT_MAPPED_MOVING_IMAGE
        return ( type = "mapped moving image" );
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::SetGradientType(GradientType type)
{
    this->m_gradientType = type;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
float
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetMaximumUpdateStepLength(void) const
{
    return this->m_maximumUpdateStepLength;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::SetMaximumUpdateStepLength(float value)
{
    if ( value>=0 )
        this->m_maximumUpdateStepLength = value;
    else
        throw std::runtime_error( "Maximum step length must be greater than or equal to 0." );
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
float
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetUpdateFieldStandardDeviation(void) const
{
    return this->m_updateFieldStandardDeviation;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::SetUpdateFieldStandardDeviation(float value)
{
    if ( value>=0 )
        this->m_updateFieldStandardDeviation = value;
    else
        throw std::runtime_error( "Standard deviation must be greater than or equal to 0." );
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
float
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetDisplacementFieldStandardDeviation(void) const
{
    return this->m_displacementFieldStandardDeviation;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::SetDisplacementFieldStandardDeviation(float value)
{
    if ( value>=0 )
        this->m_displacementFieldStandardDeviation = value;
    else
        throw std::runtime_error( "Standard deviation must be greater than or equal to 0." );
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
bool
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetUseHistogramMatching(void) const
{
    return this->m_useHistogramMatching;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::SetUseHistogramMatching(bool value)
{
    this->m_useHistogramMatching = value;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
typename DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >::TransformPointerType
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::GetInitialTransformation(void) const
{
    return this->m_initialTransform;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::SetInitialTransformation(TransformType * transform)
{
    this->m_initialTransform = transform;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
DiffeomorphicDemons< TFixedImage, TMovingImage, TTransformScalarType >
::StartRegistration(void)
{


    // Check if fixed image has been set
    if (this->m_fixedImage.IsNull())
        throw std::runtime_error( "Fixed image has not been set." );


    // Check if moving image has been set
    if (this->m_movingImage.IsNull())
        throw std::runtime_error( "Moving image has not been set." );


    // Type definition

    typedef  typename  TransformType::VectorFieldType
            VectorFieldType;

    typedef  typename  itk::MultiResolutionPDEDeformableRegistration< TFixedImage, TMovingImage, VectorFieldType, typename TFixedImage::PixelType >
            MultiResRegistrationFilterType;

    typedef  typename  itk::PDEDeformableRegistrationFilter< TFixedImage, TMovingImage, VectorFieldType >
            BaseRegistrationFilterType;


    // Local images
    typename  TFixedImage::ConstPointer   fixedImage  = this->m_fixedImage;
    typename  TMovingImage::ConstPointer  movingImage = this->m_movingImage;


    // Match the histogram between the fixed and moving images
    if ( this->m_useHistogramMatching )
    {
        // Create and set the historgam matcher
        typedef itk::HistogramMatchingImageFilter< TMovingImage, TMovingImage> MatchingFilterType;
        typename MatchingFilterType::Pointer matcher = MatchingFilterType::New();
        matcher->SetInput(                   this->m_movingImage );
        matcher->SetReferenceImage(          this->m_fixedImage );
        matcher->SetNumberOfHistogramLevels( 1024 );
        matcher->SetNumberOfMatchPoints(     7 );
        matcher->ThresholdAtMeanIntensityOn();

        // Update the matcher
        try
        {
            matcher->Update();
        }
        catch( itk::ExceptionObject& err )
        {
            throw std::runtime_error( "Could not match the histogram of input images." );
        }

        // Get the moving image
        movingImage = matcher->GetOutput();
        //movingImage->DisconnectPipeline();
    }


    // Initialize the filter
    typename BaseRegistrationFilterType::Pointer filter;
    switch ( this->m_updateRule )
    {

        case UPDATE_DIFFEOMORPHIC:
            {
                // Type definition
                typedef  typename  itk::DiffeomorphicDemonsRegistrationFilter< TFixedImage, TMovingImage, VectorFieldType >  ActualRegistrationFilterType;
                typedef  typename  ActualRegistrationFilterType::GradientType                                                   Gradient;
                // Create the "actual" registration filter, and set it to the existing filter
                typename ActualRegistrationFilterType::Pointer actualfilter = ActualRegistrationFilterType::New();
                actualfilter->SetMaximumUpdateStepLength( this->m_maximumUpdateStepLength );
                actualfilter->SetUseGradientType(         static_cast<Gradient>( this->m_gradientType ) );
                filter = actualfilter;
            }
            break;

        case UPDATE_ADDITIVE:
            {
                // Type definition
                typedef  typename  itk::FastSymmetricForcesDemonsRegistrationFilter< TFixedImage, TMovingImage, VectorFieldType>  ActualRegistrationFilterType;
                typedef  typename  ActualRegistrationFilterType::GradientType                                                        Gradient;
                // Create the "actual" registration filter, and set it to the existing filter
                typename ActualRegistrationFilterType::Pointer actualfilter = ActualRegistrationFilterType::New();
                actualfilter->SetMaximumUpdateStepLength( this->m_maximumUpdateStepLength );
                actualfilter->SetUseGradientType(         static_cast<Gradient>( this->m_gradientType ) );
                filter = actualfilter;
            }
            break;

        case UPDATE_COMPOSITIVE:
            {
                // Type definition
                typedef  typename  itk::DiffeomorphicDemonsRegistrationFilter< TFixedImage, TMovingImage, VectorFieldType >  ActualRegistrationFilterType;
                typedef  typename  ActualRegistrationFilterType::GradientType                                                   Gradient;
                // Create the "actual" registration filter, and set it to the existing filter
                typename ActualRegistrationFilterType::Pointer actualfilter = ActualRegistrationFilterType::New();
                actualfilter->SetMaximumUpdateStepLength( this->m_maximumUpdateStepLength );
                actualfilter->SetUseGradientType(         static_cast<Gradient>( this->m_gradientType ) );
                actualfilter->UseFirstOrderExpOn();
                filter = actualfilter;
            }
            break;

        default:
            throw std::runtime_error( "Unsupported update rule." );
    }


    // Set the standard deviation of the displacement field smoothing
    if ( this->m_displacementFieldStandardDeviation > 0.1 )
    {
        filter->SmoothDisplacementFieldOn();
        filter->SetStandardDeviations( this->m_displacementFieldStandardDeviation );
    }
    else
        filter->SmoothDisplacementFieldOff();


    // Set the standard deviation of the update field smoothing
    if ( this->m_updateFieldStandardDeviation > 0.1 )
    {
        filter->SmoothUpdateFieldOn();
        filter->SetUpdateFieldStandardDeviations( this->m_updateFieldStandardDeviation );
    }
    else
        filter->SmoothUpdateFieldOff();


    // This line was commented in the code of Tom
    //filter->SetIntensityDifferenceThreshold( 0.001 );


    // Define the multi-resolution filter
    typename MultiResRegistrationFilterType::Pointer multires = MultiResRegistrationFilterType::New();
    multires->SetFixedImage(         fixedImage );
    multires->SetMovingImage(        movingImage );
    multires->SetRegistrationFilter( filter );
    multires->SetNumberOfLevels(     this->m_iterations.size() );
    multires->SetNumberOfIterations( &m_iterations[0] );


    // Set the field interpolator
    typedef  itk::NearestNeighborInterpolateImageFunction< VectorFieldType, double >  FieldInterpolatorType;
    typename FieldInterpolatorType::Pointer interpolator = FieldInterpolatorType::New();
    multires->GetFieldExpander()->SetInterpolator( interpolator );


    // Set the initial displacement field only if it exists
    if (this->m_initialTransform.IsNotNull())
    {
        typename TransformType::Pointer        transform = this->m_initialTransform;
        typename VectorFieldType::ConstPointer field     = transform->GetParametersAsVectorField();
        multires->SetArbitraryInitialDisplacementField( const_cast<VectorFieldType *>(field.GetPointer()) );
    }


    // Start the registration process
    try
    {
        multires->UpdateLargestPossibleRegion();
    }
    catch( itk::ExceptionObject& err )
    {
        std::string message = "Unexpected error: ";
        message += err.GetDescription();
        throw std::runtime_error( message  );
    }

    // Set the displacement field to the transformation object
    static_cast< TransformType * >(this->m_transform.GetPointer())->SetParametersAsVectorField(
            static_cast<typename VectorFieldType::ConstPointer>(multires->GetOutput()));
}


} // End of namespace


#endif // _RPI_DIFFEOMORPHIC_DEMONS_CXX_
