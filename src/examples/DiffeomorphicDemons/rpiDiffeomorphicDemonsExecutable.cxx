#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <tclap/CmdLine.h>
#include <rpiCommonTools.hxx>
#include "rpiDiffeomorphicDemons.hxx"



/**
 * Diffeomorphic demons executable.
 * @author Vincent Garcia and Tom Vercauteren
 * @date   2010/10/29
 */



/**
 * Structure containing the parameters.
 */
struct Param{
    std::string  fixedImagePath;
    std::string  movingImagePath;
    std::string  outputImagePath;
    std::string  outputTransformPath;
    std::string  intialLinearTransformPath;
    std::string  intialFieldTransformPath;
    std::string  iterations;
    unsigned int updateRule;
    float        maximumUpdateStepLength;
    unsigned int gradientType;
    float        updateFieldStandardDeviation;
    float        displacementFieldStandardDeviation;
    bool         useHistogramMatching;
    rpi::ImageInterpolatorType interpolatorType;
};



/**
 * Parses the command line arguments and deduces the corresponding Param structure.
 * @param  argc   number of arguments
 * @param  argv   array containing the arguments
 * @param  param  structure of parameters
 */
void parseParameters(int argc, char** argv, struct Param & param)
{
    // Program description
    std::string description = "\b\b\bDESCRIPTION\n";
    description += "Diffeomorphic demons registration method. ";
    description += "The transformation computed is a dense displacement field.";
    description += "\nAuthors : Vincent Garcia and Tom Vercauteren";

    // Option description
    std::string des_interpolatorType;
    des_interpolatorType  = "Type of image interpolator used for resampling the moving image at the ";
    des_interpolatorType += "very end of the program. Must be an integer value among 0 = nearest ";
    des_interpolatorType += "neighbor, 1 = linear, 2 = b-spline, and 3 = sinus cardinal (default 1).";

    std::string des_useHistogramMatching = "Use histogram matching before processing? (default false). ";

    std::string des_disFieldSigma        = "Standard deviation of the Gaussian smoothing of the displacement field (voxel units). ";
    des_disFieldSigma                   += "Setting it below 0.1 means no smoothing will be performed (default 1.5).";

    std::string des_upFieldSigma         = "Standard deviation of the Gaussian smoothing of the update field (voxel units). ";
    des_upFieldSigma                    += "Setting it below 0.1 means no smoothing will be performed (default 0.0).";

    std::string des_gradientType         = "Type of gradient used for computing the demons force. ";
    des_gradientType                    += "0 is symmetrized, 1 is fixed image, 2 is warped moving image, 3 is mapped moving image (default 0).";

    std::string des_maxStepLength        = "Maximum length of an update vector (voxel units). ";
    des_maxStepLength                   += "Setting it to 0 implies no restrictions will be made on the step length.";

    std::string des_updateRule           = "Update rule:  0: s <- s o exp(u) (diffeomorphic) ; ";
    des_updateRule                      += "1: s <- s + u (additive, ITK basic); 2: s <- s o (Id+u) (compositive, Thirion's proposal?) (default 0).";

    std::string des_iterations           = "Number of iterations per level of resolution (from coarse to fine levels). ";
    des_iterations                      += "Levels must be separated by \"x\" (default 15x10x5).";

    std::string des_initLinearTransform  = "Path to the initial linear transformation.";

    std::string des_initFieldTransform   = "Path to the initial displacement field transformation.";

    std::string des_outputImage          = "Path to the output image (default output_image.nii).";

    std::string des_outputTransform      = "Path of the output transformation (default output_transform.nii).";

    std::string des_movingImage          = "Path to the moving image.";

    std::string des_fixedImage           = "Path to the fixed image.";


    try {

        // Define the command line parser
        TCLAP::CmdLine cmd( description, ' ', "1.0", true );

        // Set options
        TCLAP::ValueArg<unsigned int> arg_interpolatorType( "", "interpolator-type", des_interpolatorType, false, 1, "int", cmd);
        TCLAP::SwitchArg              arg_useHistogramMatching( "",  "use-histogram-matching", des_useHistogramMatching, cmd, false);
        TCLAP::ValueArg<float>        arg_disFieldSigma( "d", "displacement-field-sigma", des_disFieldSigma, false, 1.5, "float", cmd );
        TCLAP::ValueArg<float>        arg_upFieldSigma( "u", "update-field-sigma", des_upFieldSigma, false, 0.0, "float", cmd );
        TCLAP::ValueArg<unsigned int> arg_gradientType( "g", "gradient-type", des_gradientType, false, 0, "uint", cmd );
        TCLAP::ValueArg<float>        arg_maxStepLength( "l", "max-step-length", des_maxStepLength, false, 2.0, "float", cmd );
        TCLAP::ValueArg<unsigned int> arg_updateRule( "r", "update-rule", des_updateRule, false, 0, "uint", cmd );
        TCLAP::ValueArg<std::string>  arg_iterations( "a", "iterations", des_iterations, false, "15x10x5", "uintxuintx...xuint", cmd );
        TCLAP::ValueArg<std::string>  arg_initLinearTransform( "", "initial-linear-transform", des_initLinearTransform, false, "", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_initFieldTransform( "", "initial-transform", des_initFieldTransform,  false, "", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_outputImage( "i", "output-image", des_outputImage, false, "output_image.nii", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_outputTransform( "t", "output-transform", des_outputTransform, false, "output_transform.nii", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_movingImage( "m", "moving-image", des_movingImage, true, "", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_fixedImage( "f", "fixed-image", des_fixedImage, true, "", "string", cmd );

        // Parse the command line
        cmd.parse( argc, argv );

        // Set the parameters
        param.fixedImagePath                     = arg_fixedImage.getValue();
        param.movingImagePath                    = arg_movingImage.getValue();
        param.outputTransformPath                = arg_outputTransform.getValue();
        param.outputImagePath                    = arg_outputImage.getValue();
        param.intialLinearTransformPath          = arg_initLinearTransform.getValue();
        param.intialFieldTransformPath           = arg_initFieldTransform.getValue();
        param.iterations                         = arg_iterations.getValue();
        param.updateRule                         = arg_updateRule.getValue();
        param.maximumUpdateStepLength            = arg_maxStepLength.getValue();
        param.gradientType                       = arg_gradientType.getValue();
        param.updateFieldStandardDeviation       = arg_upFieldSigma.getValue();
        param.displacementFieldStandardDeviation = arg_disFieldSigma.getValue();
        param.useHistogramMatching               = arg_useHistogramMatching.getValue();

        // Set the interpolator type
        unsigned int interpolator_type = arg_interpolatorType.getValue();
        if      ( interpolator_type==0 )
            param.interpolatorType = rpi::INTERPOLATOR_NEAREST_NEIGHBOR;
        else if ( interpolator_type==1 )
            param.interpolatorType = rpi::INTERPOLATOR_LINEAR;
       else if  ( interpolator_type==2 )
            param.interpolatorType = rpi::INTERPOLATOR_BSLPINE;
        else if ( interpolator_type==3 )
            param.interpolatorType = rpi::INTERPOLATOR_SINUS_CARDINAL;
        else
            throw std::runtime_error("Image interpolator not supported.");
    }
    catch (TCLAP::ArgException &e)
    {
        std::cerr << "Error: " << e.error() << " for argument " << e.argId() << std::endl;
        throw std::runtime_error("Unable to parse the command line arguments.");
    }
}



/**
  * Prints parameters.
  * @param  fixedImagePath             path to the fixed image
  * @param  movingImagePath            path to the moving image
  * @param  outputImagePath            path to the output file containing the resampled image
  * @param  outputTransformPath        path to the output file containing the transformation
  * @param  initialLinearTransformPath path to the output file containing the transformation
  * @param  initialFieldTransformPath  path to the output file containing the transformation
  * @param  interpolatorType           interpolator type
  * @param  registration               registration object
  */
template< class TFixedImage, class TMovingImage, class TTransformScalarType >
void PrintParameters( std::string fixedImagePath,
                      std::string movingImagePath,
                      std::string outputImagePath,
                      std::string outputTransformPath,
                      std::string initialLinearTransformPath,
                      std::string initialFieldTransformPath,
                      rpi::ImageInterpolatorType interpolatorType,
                      rpi::DiffeomorphicDemons<TFixedImage, TMovingImage, TTransformScalarType> * registration )
{
    // Print I/O parameters
    std::cout << std::endl;
    std::cout << "I/O PARAMETERS"                             << std::endl;
    std::cout << "  Fixed image path                      : " << fixedImagePath      << std::endl;
    std::cout << "  Moving image path                     : " << movingImagePath     << std::endl;
    std::cout << "  Output image path                     : " << outputImagePath     << std::endl;
    std::cout << "  Output transformation path            : " << outputTransformPath << std::endl;
    if ( initialLinearTransformPath.compare("")!=0 )
        std::cout << "  Initial linear transform              : " << initialLinearTransformPath << std::endl;
    if ( initialFieldTransformPath.compare("")!=0 )
        std::cout << "  Initial displacement field transform  : " << initialFieldTransformPath  << std::endl;
    std::cout << std::endl;

    // Print method parameters
    std::cout << "METHOD PARAMETERS"                          << std::endl;
    std::cout << "  Iterations                            : " << rpi::VectorToString<unsigned int>( registration->GetNumberOfIterations() ) << std::endl;
    std::cout << "  Update rule                           : " << registration->GetUpdateRuleAsString()                                      << std::endl;
    std::cout << "  Maximum step length                   : " << registration->GetMaximumUpdateStepLength()              << " (voxel unit)" << std::endl;
    std::cout << "  Gradient type                         : " << registration->GetGradientTypeAsString()                                    << std::endl;
    std::cout << "  Update field standard deviation       : " << registration->GetUpdateFieldStandardDeviation()         << " (voxel unit)" << std::endl;
    std::cout << "  Displacement field standard deviation : " << registration->GetDisplacementFieldStandardDeviation()   << " (voxel unit)" << std::endl;
    std::cout << "  Use histogram matching?               : " << rpi::BooleanToString( registration->GetUseHistogramMatching() )            << std::endl;
    std::cout << "  Interpolator type                     : " << rpi::getImageInterpolatorTypeAsString(interpolatorType)                    << std::endl;
    std::cout << std::endl;
}



/**
  * Starts the image registration.
  * @param   param  parameters needed for the image registration process
  * @return  EXIT_SUCCESS if the registration succeded, EXIT_FAILURE otherwise
  */
template< class TFixedImage, class TMovingImage >
int StartMainProgram(struct Param param)
{

    typedef float
            TransformScalarType;

    typedef rpi::DiffeomorphicDemons< TFixedImage, TMovingImage, TransformScalarType >
            RegistrationMethod;

    typedef itk::Transform<double, 3, 3>
            LinearTransformType;

    typedef rpi::DisplacementFieldTransform<TransformScalarType, 3>
            FieldTransformType;


    // Creation of the registration object
    RegistrationMethod * registration = new RegistrationMethod();


    try
    {
        // Read input images
        typename TFixedImage::Pointer  fixedImage  = rpi::readImage< TFixedImage >(  param.fixedImagePath );
        typename TMovingImage::Pointer movingImage = rpi::readImage< TMovingImage >( param.movingImagePath );


        // Set parameters
        registration->SetFixedImage(                         fixedImage );
        registration->SetMovingImage(                        movingImage );
        registration->SetNumberOfIterations(                 rpi::StringToVector<unsigned int>( param.iterations ) );
        registration->SetMaximumUpdateStepLength(            param.maximumUpdateStepLength );
        registration->SetUpdateFieldStandardDeviation(       param.updateFieldStandardDeviation );
        registration->SetDisplacementFieldStandardDeviation( param.displacementFieldStandardDeviation );
        registration->SetUseHistogramMatching(               param.useHistogramMatching );


        // Set update rule
        switch( param.updateRule )
        {
        case 0:
            registration->SetUpdateRule( RegistrationMethod::UPDATE_DIFFEOMORPHIC ); break;
        case 1:
            registration->SetUpdateRule( RegistrationMethod::UPDATE_ADDITIVE );      break;
        case 2:
            registration->SetUpdateRule( RegistrationMethod::UPDATE_COMPOSITIVE );   break;
        default:
            throw std::runtime_error( "Update rule must fit in the range [0,2]." );
        }


        // Set gradient type
        switch( param.gradientType )
        {
        case 0:
            registration->SetGradientType( RegistrationMethod::GRADIENT_SYMMETRIZED );         break;
        case 1:
            registration->SetGradientType( RegistrationMethod::GRADIENT_FIXED_IMAGE );         break;
        case 2:
            registration->SetGradientType( RegistrationMethod::GRADIENT_WARPED_MOVING_IMAGE ); break;
        case 3:
            registration->SetGradientType( RegistrationMethod::GRADIENT_MAPPED_MOVING_IMAGE ); break;
        default:
            throw std::runtime_error( "Gradient type must fit in the range [0,3]." );
        }


        // Set initialize transformation
        if ( param.intialFieldTransformPath.compare("")!=0  &&  param.intialLinearTransformPath.compare("")!=0 )
        {
            throw std::runtime_error( "Cannot initialize with a displacement field and a linear transformation." );
        }
        else if ( param.intialFieldTransformPath.compare("")!=0 )
        {
            typename FieldTransformType::Pointer field = rpi::readDisplacementField<TransformScalarType>( param.intialFieldTransformPath );
            registration->SetInitialTransformation( field );
        }
        else if ( param.intialLinearTransformPath.compare("")!=0 )
        {
            typename LinearTransformType::Pointer linear = rpi::readLinearTransformation<double>( param.intialLinearTransformPath );
            typename FieldTransformType::Pointer  field  = rpi::linearToDisplacementFieldTransformation<double,TransformScalarType, TFixedImage>( fixedImage, linear );
            registration->SetInitialTransformation( field );
        }


        // Print parameters
        PrintParameters<TFixedImage, TMovingImage, TransformScalarType>(
                    param.fixedImagePath,
                    param.movingImagePath,
                    param.outputImagePath,
                    param.outputTransformPath,
                    param.intialLinearTransformPath,
                    param.intialFieldTransformPath,
                    param.interpolatorType,
                    registration );


        // Display
        std::cout << "STARTING MAIN PROGRAM" << std::endl;


        // Start registration process
        std::cout << "  Registering images                    : " << std::flush;
        registration->StartRegistration();
        std::cout << "OK" << std::endl;


        // Write the output transformation
        std::cout << "  Writing transformation                : " << std::flush;
        rpi::writeDisplacementFieldTransformation<TransformScalarType, TFixedImage::ImageDimension>(
                    registration->GetTransformation(),
                    param.outputTransformPath );
        std::cout << "OK" << std::endl;


        // Write the output image
        std::cout << "  Writing image                         : " << std::flush;
        rpi::resampleAndWriteImage<TFixedImage, TMovingImage, TransformScalarType>(
                    fixedImage,
                    movingImage,
                    registration->GetTransformation(),
                    param.outputImagePath,
                    param.interpolatorType );
        std::cout << "OK" << std::endl << std::endl;
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        delete registration;
        return EXIT_FAILURE;
    };


    delete registration;
    return EXIT_SUCCESS;
}



/**
 * Main function.
 */
int main(int argc, char** argv)
{
    // Parse parameters
    struct Param param;
    parseParameters( argc, argv, param);


    // Read image information
    itk::ImageIOBase::Pointer fixed_imageIO;
    itk::ImageIOBase::Pointer moving_imageIO;
    try
    {
        fixed_imageIO  = rpi::readImageInformation( param.fixedImagePath );
        moving_imageIO = rpi::readImageInformation( param.movingImagePath );
    }
    catch( std::exception& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    // Only 3D images are supported yet
    if (  fixed_imageIO->GetNumberOfDimensions()!=3  &&  moving_imageIO->GetNumberOfDimensions()!=3  )
    {
        std::cerr << "Error: Only images of dimension 3 are supported yet." << std::endl;
        return EXIT_FAILURE;
    }


    // Only scalar images are supported yet
    if (  fixed_imageIO->GetPixelType() != itk::ImageIOBase::SCALAR  ||  moving_imageIO->GetPixelType() != itk::ImageIOBase::SCALAR  )
    {
        std::cerr << "Error: Only scalar images are supported yet." << std::endl;
        return EXIT_FAILURE;
    }


    // Start main program
    return StartMainProgram< itk::Image<float,3> , itk::Image<float,3> >( param );
}
