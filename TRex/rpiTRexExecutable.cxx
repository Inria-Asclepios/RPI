#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <tclap/CmdLine.h>
#include <mipsInrimageImageIOFactory.h>
#include <rpiCommonTools.hxx>
#include "rpiTRex.hxx"



/**
 * TRex (Toy Registration EXample) executable.
 * This method is a fully ITK registration method implemented into the RPI framework.
 * The method is simple and gives a clear example of how a registration method of RPI should be implemented.
 * @author Vincent Garcia
 * @date   2011/03/15
 */



/**
 * Structure containing the parameters.
 */
struct Param{
    std::string  fixedImagePath;
    std::string  movingImagePath;
    std::string  outputImagePath;
    std::string  outputTransformPath;
    unsigned int iterations;
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
    description += "TRex registration method. ";
    description += "\nAuthor : Vincent Garcia";

    try {

        // Define the command line parser
        TCLAP::CmdLine cmd( description, ' ', "1.0", true);

        TCLAP::ValueArg<unsigned int> arg_iterations( "a", "iterations", "Number of iterations (default 5000)", false, 5000, "uint", cmd );
        TCLAP::ValueArg<std::string>  arg_outputImage( "i", "output-image", "Path to the output image (default output_image.nii).", false, "output_image.nii", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_outputTransform( "t", "output-transform", "Path of the output transformation (default output_transform.txt).", false, "output_transform.txt", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_movingImage( "m", "moving-image", "Path to the moving image.", true, "", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_fixedImage( "f", "fixed-image", "Path to the fixed image.", true, "", "string", cmd );

        // Parse the command line
        cmd.parse( argc, argv );

        // Set the parameters
        param.fixedImagePath      = arg_fixedImage.getValue();
        param.movingImagePath     = arg_movingImage.getValue();
        param.outputTransformPath = arg_outputTransform.getValue();
        param.outputImagePath     = arg_outputImage.getValue();
        param.iterations          = arg_iterations.getValue();

    }
    catch (TCLAP::ArgException &e)
    {
        std::cerr << "Error: " << e.error() << " for argument " << e.argId() << std::endl;
        throw std::runtime_error("Unable to parse the command line arguments.");
    }
}


/**
  * Prints parameters.
  * @param  fixedImagePath       path to the fixed image
  * @param  movingImagePath      path to the moving image
  * @param  outputImagePath      path to the output file containing the resampled image
  * @param  outputTransformPath  path to the output file containing the transformation
  * @param  initialTransformPath path to the initial transformation
  * @param  registration         registration object
  */
template< class TFixedImage, class TMovingImage, class TTransformScalarType >
void PrintParameters( std::string fixedImagePath,
                      std::string movingImagePath,
                      std::string outputImagePath,
                      std::string outputTransformPath,
                      rpi::TRex<TFixedImage, TMovingImage, TTransformScalarType> * registration )
{

    // Print I/O parameters
    std::cout << std::endl;
    std::cout << "I/O PARAMETERS"                  << std::endl;
    std::cout << "  Fixed image path           : " << fixedImagePath      << std::endl;
    std::cout << "  Moving image path          : " << movingImagePath     << std::endl;
    std::cout << "  Output image path          : " << outputImagePath     << std::endl;
    std::cout << "  Output transformation path : " << outputTransformPath << std::endl;
    std::cout << std::endl;

    // Print method parameters
    std::cout << "METHOD PARAMETER"                << std::endl;
    std::cout << "  Number of iterations       : " << registration->GetNumberOfIterations()     << std::endl << std::endl;
}


/**
  * Starts the main program.
  * @param   param  parameters needed for the image registration process
  * @return  EXIT_SUCCESS if the registration succeded, EXIT_FAILURE otherwise
  */
template< class TFixedImage, class TMovingImage >
int StartMainProgram(struct Param param)
{

    // Type definition
    typedef double                                                      TransformScalarType;
    typedef rpi::TRex< TFixedImage, TMovingImage, TransformScalarType > RegistrationMethod;

    // Create registration object
    RegistrationMethod * registration = new RegistrationMethod();

    try
    {
        // Read input images
        typename TFixedImage::Pointer  fixedImage  = rpi::readImage< TFixedImage >(  param.fixedImagePath );
        typename TMovingImage::Pointer movingImage = rpi::readImage< TMovingImage >( param.movingImagePath );

        // Set parameters
        registration->SetFixedImage(         fixedImage );
        registration->SetMovingImage(        movingImage );
        registration->SetNumberOfIterations( param.iterations );

        // Print parameters
        PrintParameters<TFixedImage, TMovingImage, TransformScalarType>(
                param.fixedImagePath,
                param.movingImagePath,
                param.outputImagePath,
                param.outputTransformPath,
                registration );

        // Display
        std::cout << "STARTING MAIN PROGRAM" << std::endl;

        // Start registration process
        std::cout << "  Registering images         : " << std::flush;
        registration->StartRegistration();
        std::cout << "OK" << std::endl;

        // Write output transformation
        std::cout << "  Writting transformation    : " << std::flush;
        rpi::writeLinearTransformation<TransformScalarType, TFixedImage::ImageDimension>(
                registration->GetTransformation(),
                param.outputTransformPath );
        std::cout << "OK" << std::endl;

        // Write output image
        std::cout << "  Writting image             : " << std::flush;
        rpi::writeImage<TFixedImage, TMovingImage, TransformScalarType>(
                registration->GetTransformation(),
                fixedImage,
                movingImage,
                param.outputImagePath );
        std::cout << "OK" << std::endl << std::endl;
    }
    catch( std::exception& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
        delete registration;
        return EXIT_FAILURE;
    };

    // Free memory and exit
    delete registration;
    return EXIT_SUCCESS;
}



/**
 * Main function.
 */
int main(int argc, char** argv)
{

    // Allows the executable to read and write Inrimage
    itk::InrimageImageIOFactory::RegisterOneFactory();


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


    // Component type
    typedef itk::ImageIOBase::IOComponentType ComponentType;
    ComponentType  fixed_type      = fixed_imageIO->GetComponentType();
    ComponentType  moving_type     = moving_imageIO->GetComponentType();
    std::string    fixed_type_str  = fixed_imageIO->GetComponentTypeAsString( fixed_type );
    std::string    moving_type_str = moving_imageIO->GetComponentTypeAsString( moving_type );


    // Switch on component type
    switch ( fixed_type )
    {
        case itk::ImageIOBase::UCHAR :
            switch ( moving_type )
            {
                case itk::ImageIOBase::UCHAR :
                    return StartMainProgram< itk::Image<unsigned char, 3> , itk::Image<unsigned char,  3> >(param); break;
                case itk::ImageIOBase::USHORT :
                    return StartMainProgram< itk::Image<unsigned char, 3> , itk::Image<unsigned short, 3> >(param); break;
                case itk::ImageIOBase::SHORT :
                    return StartMainProgram< itk::Image<unsigned char, 3> , itk::Image<short,          3> >(param); break;
                case itk::ImageIOBase::FLOAT :
                    return StartMainProgram< itk::Image<unsigned char, 3> , itk::Image<float,          3> >(param); break;
                default :
                    std::cerr << "Error: Pixel type " << moving_type_str << " is not supported yet." << std::endl;
                    return EXIT_FAILURE;
            }
            break;

        case itk::ImageIOBase::USHORT :
            switch ( moving_type )
            {
                case itk::ImageIOBase::UCHAR :
                    return StartMainProgram< itk::Image<unsigned short, 3> , itk::Image<unsigned char,  3> >(param); break;
                case itk::ImageIOBase::USHORT :
                    return StartMainProgram< itk::Image<unsigned short, 3> , itk::Image<unsigned short, 3> >(param); break;
                case itk::ImageIOBase::SHORT :
                    return StartMainProgram< itk::Image<unsigned short, 3> , itk::Image<short,          3> >(param); break;
                case itk::ImageIOBase::FLOAT :
                    return StartMainProgram< itk::Image<unsigned short, 3> , itk::Image<float,          3> >(param); break;
                default :
                    std::cerr << "Error: Pixel type " << moving_type_str << " is not supported yet." << std::endl;
                    return EXIT_FAILURE;
            }
            break;

        case itk::ImageIOBase::SHORT :
            switch ( moving_type )
            {
                case itk::ImageIOBase::UCHAR :
                    return StartMainProgram< itk::Image<short, 3> , itk::Image<unsigned char,  3> >(param); break;
                case itk::ImageIOBase::USHORT :
                    return StartMainProgram< itk::Image<short, 3> , itk::Image<unsigned short, 3> >(param); break;
                case itk::ImageIOBase::SHORT :
                    return StartMainProgram< itk::Image<short, 3> , itk::Image<short,          3> >(param); break;
                case itk::ImageIOBase::FLOAT :
                    return StartMainProgram< itk::Image<short, 3> , itk::Image<float,          3> >(param); break;
                default :
                    std::cerr << "Error: Pixel type " << moving_type_str << " is not supported yet." << std::endl;
                    return EXIT_FAILURE;
            }
            break;

        case itk::ImageIOBase::FLOAT :
            switch ( moving_type )
            {
                case itk::ImageIOBase::UCHAR :
                    return StartMainProgram< itk::Image<float, 3> , itk::Image<unsigned char,  3> >(param); break;
                case itk::ImageIOBase::USHORT :
                    return StartMainProgram< itk::Image<float, 3> , itk::Image<unsigned short, 3> >(param); break;
                case itk::ImageIOBase::SHORT :
                    return StartMainProgram< itk::Image<float, 3> , itk::Image<short,          3> >(param); break;
                case itk::ImageIOBase::FLOAT :
                    return StartMainProgram< itk::Image<float, 3> , itk::Image<float,          3> >(param); break;
                default :
                    std::cerr << "Error: Pixel type " << moving_type_str << " is not supported yet." << std::endl;
                    return EXIT_FAILURE;
            }
            break;

        default :
            std::cerr << "Error: Pixel type " << fixed_type_str << " is not supported yet." << std::endl;
            return EXIT_FAILURE;
    }


    // End of the program
    return EXIT_FAILURE;
}
