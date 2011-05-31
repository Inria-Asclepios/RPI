#ifndef _RPI_TREX_CXX_
#define _RPI_TREX_CXX_

#include <itkMattesMutualInformationImageToImageMetric.h>
#include <itkImageRegistrationMethod.h>
#include <itkCenteredTransformInitializer.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkRegularStepGradientDescentOptimizer.h>

#include "rpiTRex.hxx"



// Namespace RPI : Registration Programming Interface
namespace rpi
{



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
TRex< TFixedImage, TMovingImage, TTransformScalarType >
::TRex(void)
{

    // Initialization
    this->m_iterations = 5000;
    this->m_transform  = TransformType::New();
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
TRex< TFixedImage, TMovingImage, TTransformScalarType >
::~TRex(void)
{
    // Do nothing
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
unsigned int
TRex< TFixedImage, TMovingImage, TTransformScalarType >
::GetNumberOfIterations(void) const
{
    return this->m_iterations;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
TRex< TFixedImage, TMovingImage, TTransformScalarType >
::SetNumberOfIterations(unsigned int value)
{
    this->m_iterations = value;
}



template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
TRex< TFixedImage, TMovingImage, TTransformScalarType >
::StartRegistration(void)
{


    // Check if fixed image has been set
    if (this->m_fixedImage.IsNull())
        throw std::runtime_error( "Fixed image has not been set." );


    // Check if moving image has been set
    if (this->m_movingImage.IsNull())
        throw std::runtime_error( "Moving image has not been set." );


    // Type definition
    typedef itk::RegularStepGradientDescentOptimizer                                    OptimizerType;
    typedef itk::MattesMutualInformationImageToImageMetric< TFixedImage, TMovingImage > MetricType ;
    typedef itk::LinearInterpolateImageFunction< TMovingImage, double >                 InterpolatorType;
    typedef itk::ImageRegistrationMethod< TFixedImage, TMovingImage >                   RegistrationType;


    // Create the metric, the optimizer, the interpolator, and the registration objects
    typename MetricType::Pointer       metric       = MetricType::New();
    typename OptimizerType::Pointer    optimizer    = OptimizerType::New();
    typename InterpolatorType::Pointer interpolator = InterpolatorType::New();
    typename RegistrationType::Pointer registration = RegistrationType::New();


    // Initialize the registration method
    registration->SetMetric(           metric );
    registration->SetOptimizer(        optimizer );
    registration->SetInterpolator(     interpolator );
    registration->SetFixedImage(       this->m_fixedImage );
    registration->SetMovingImage(      this->m_movingImage );
    registration->SetFixedImageRegion( this->m_fixedImage->GetBufferedRegion() );


    // Initialize the transformation
    typedef itk::CenteredTransformInitializer< TransformType, TFixedImage, TMovingImage >  TransformInitializerType;
    typename TransformType::Pointer            transform   = TransformType::New();
    typename TransformInitializerType::Pointer initializer = TransformInitializerType::New();
    initializer->SetTransform(   transform );
    initializer->SetFixedImage(  this->m_fixedImage );
    initializer->SetMovingImage( this->m_movingImage );
    initializer->GeometryOn(); // It's either GeometryOn (center of image) or MomentsOn (center of mass)
    initializer->InitializeTransform();
    registration->SetTransform(                  transform );
    registration->SetInitialTransformParameters( transform->GetParameters() );


    // Initialize the metric
    metric->SetNumberOfHistogramBins(  50 );
    metric->SetNumberOfSpatialSamples( 50000 );
    
    
    // Initialize the scale of the optimizer
    typedef OptimizerType::ScalesType OptimizerScalesType;
    OptimizerScalesType optimizerScales( transform->GetNumberOfParameters() );
    const double translation_scale = 1.0 / 20.0;
    optimizerScales[0] = 1.0;
    optimizerScales[1] = 1.0;
    optimizerScales[2] = 1.0;
    optimizerScales[3] = translation_scale;
    optimizerScales[4] = translation_scale;
    optimizerScales[5] = translation_scale;
    optimizer->SetScales( optimizerScales );


    // Initialize the other variables of the optimizer
    optimizer->SetRelaxationFactor(   0.6 );
    optimizer->SetMaximumStepLength(  0.1 );
    optimizer->SetMinimumStepLength(  0.001 );
    optimizer->SetNumberOfIterations( this->m_iterations );


    // Start the registration process
    try
    {
        registration->UpdateLargestPossibleRegion();
    }
    catch( itk::ExceptionObject & err )
    {
        std::cout << err << std::endl;
        throw std::runtime_error( "Unexpected error." );
    }

    // Set the transformation parameters
    static_cast< TransformType * >(this->m_transform.GetPointer())->SetCenter(     transform->GetCenter() );
    static_cast< TransformType * >(this->m_transform.GetPointer())->SetParameters( registration->GetLastTransformParameters() );
}


} // End of namespace


#endif // _RPI_TREX_CXX_
