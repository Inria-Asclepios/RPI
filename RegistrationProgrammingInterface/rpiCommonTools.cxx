#ifndef _RPI_COMMON_TOOLS_CXX_
#define _RPI_COMMON_TOOLS_CXX_

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <cerrno>
#include <climits>

#include <itkImage.h>
#include <itkImageIOBase.h>
#include <itkImageIOFactory.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkTransform.h>
#include <itkAffineTransform.h>
#include <itkEuler2DTransform.h>
#include <itkEuler3DTransform.h>
#include <itkTransformFileReader.h>
#include <itkTransformFileWriter.h>
#include <itkTransformToDeformationFieldSource.h>

#include <itkResampleImageFilter.h>
#include <itkWarpImageFilter.h>

#include "rpiCommonTools.hxx"



// Namespace RPI : Registration Programming Interface
namespace rpi
{



inline itk::ImageIOBase::Pointer readImageInformation( std::string fileName )
{

    // Define image IO
    itk::ImageIOBase::Pointer imageIO  = itk::ImageIOFactory::CreateImageIO( fileName.c_str(), itk::ImageIOFactory::ReadMode );

    // Test if image exists
    if ( !imageIO )
        throw std::runtime_error( "Could not read image " + fileName + "." );

    // Read image information
    try
    {
        imageIO->SetFileName( fileName );
        imageIO->ReadImageInformation();
    }
    catch( itk::ExceptionObject&  )
    {
        throw std::runtime_error( "Could not read image information." );
    }
    return imageIO;
}



template< typename TImage >
typename TImage::Pointer  readImage( std::string fileName )
{
    typedef itk::ImageFileReader< TImage >  ImageReaderType;
    typename ImageReaderType::Pointer reader = ImageReaderType::New();
    reader->SetFileName( fileName );
    try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject& )
    {
        throw std::runtime_error( "Could not read the input image." );
    }
    return reader->GetOutput();
}


template< class TTransformScalarType>
typename itk::Transform< TTransformScalarType, 3, 3 >::Pointer
read3DTransformation( std::string fileName )
{

    // Type definition
    typedef  itk::TransformFileReader                TransformReaderType;
    typedef  TransformReaderType::TransformListType  TransformListType;
    typedef  TransformReaderType::TransformType      BaseTransformType;

    // Dimension
    const int TDimension = 3;

    // Define transformation reader
    TransformReaderType::Pointer reader = TransformReaderType::New();
    reader->SetFileName(  fileName );

    // Update the reader
    try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject& )
    {
        throw std::runtime_error( "Could not read the input transformation." );
    }

    // Get the list of transformations
    TransformListType * list = reader->GetTransformList();
    if ( list->empty() )
        throw std::runtime_error( "The input file does not contain any transformation." );
    else if ( list->size()>1 )
        throw std::runtime_error( "The input file contains more than one transformation." );

    // Get a pointer on the transformation
    BaseTransformType * transform = list->front().GetPointer();
    if ( !strcmp( transform->GetNameOfClass(), "AffineTransform" ) )
        return static_cast< itk::AffineTransform< TTransformScalarType, TDimension > * >( transform );
    if ( !strcmp( transform->GetNameOfClass(), "Euler3DTransform" ) )
        return static_cast< itk::Euler3DTransform< TTransformScalarType > * >( transform );
    else
        throw std::runtime_error( "Transformation type not supported yet." );

    return 0;
}


template< class TTransformScalarType>
typename itk::Euler3DTransform< TTransformScalarType >::Pointer
readEuler3DTransformation( std::string fileName )
{

    // Type definition
    typedef  itk::TransformFileReader                TransformReaderType;
    typedef  TransformReaderType::TransformListType  TransformListType;
    typedef  TransformReaderType::TransformType      BaseTransformType;

    // Define transformation reader
    TransformReaderType::Pointer reader = TransformReaderType::New();
    reader->SetFileName(  fileName );

    // Update the reader
   try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject& err )
    {
        throw std::runtime_error( "Could not read the input transformation." );
    }

    // Get the list of transformations
    TransformListType * list = reader->GetTransformList();
    if ( list->empty() )
        throw std::runtime_error( "The input file does not contain any transformation." );
    else if ( list->size()>1 )
        throw std::runtime_error( "The input file contains more than one transformation." );

    // Get a pointer on the transformation
    BaseTransformType * transform = list->front().GetPointer();
    if ( !strcmp( transform->GetNameOfClass(), "Euler3DTransform" ) )
        return static_cast< itk::Euler3DTransform< TTransformScalarType > * >( transform );
    else
        throw std::runtime_error( "Transformation type not supported yet." );

    return 0;
}



template< class TTransformScalarType, int TDimension >
typename itk::DisplacementFieldTransform< TTransformScalarType, TDimension >::Pointer
readDisplacementField( std::string fileName )
{

    typedef itk::DisplacementFieldTransform< TTransformScalarType, TDimension >
            FieldTransformType;

    typedef typename FieldTransformType::DisplacementFieldType
            FieldContainerType;

    typedef itk::ImageFileReader< FieldContainerType >
            ReaderType;

    // Read the displacement field (image)
    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( fileName );
    try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject& )
    {
        throw std::runtime_error( "Could not read the input transformation." );
    }
    typename FieldContainerType::Pointer field = reader->GetOutput();

    // Create the displacement field (transformation)
    typename FieldTransformType::Pointer transform = FieldTransformType::New();
    transform->SetDisplacementField( static_cast<typename FieldContainerType::ConstPointer>( field.GetPointer() ) );
    return transform;
}



/*template< class TTransformScalarType, int TDimension >
typename itk::StationaryVelocityFieldTransform< TTransformScalarType, TDimension >::Pointer
readStationaryVelocityField( std::string fileName )
{

    typedef itk::StationaryVelocityFieldTransform< TTransformScalarType, TDimension >
            FieldTransformType;

    typedef typename FieldTransformType::VelocityFieldType
            FieldContainerType;

    typedef itk::ImageFileReader< FieldContainerType >
            ReaderType;

    // Read the velocity field (image)
    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( fileName );
    try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject& err )
    {
        throw std::runtime_error( "Could not read the input transformation." );
    }
    typename FieldContainerType::Pointer field = reader->GetOutput();

    // Create the displacement field (transformation)
    typename FieldTransformType::Pointer transform = FieldTransformType::New();
    transform->SetVelocityField( static_cast<typename FieldContainerType::ConstPointer>( field.GetPointer() ) );
    return transform;
}
*/


template< class TTransformScalarType, class TImage >
typename itk::DisplacementFieldTransform< TTransformScalarType, TImage::ImageDimension >::Pointer
linearToDisplacementFieldTransformation( TImage * image, itk::Transform< TTransformScalarType, TImage::ImageDimension, TImage::ImageDimension > * linearTransform )
{
    // Type definition
    typedef  itk::DisplacementFieldTransform< TTransformScalarType, TImage::ImageDimension >     FieldTransformType;
    typedef  typename  FieldTransformType::DisplacementFieldType                                 FieldContainerType;
    typedef  itk::TransformToDeformationFieldSource< FieldContainerType, TTransformScalarType >  GeneratorType;

    // Create a field generator
    typename GeneratorType::Pointer fieldGenerator = GeneratorType::New();
    fieldGenerator->SetTransform(       linearTransform );
    fieldGenerator->SetOutputRegion(    image->GetRequestedRegion() );
    fieldGenerator->SetOutputOrigin(    image->GetOrigin() );
    fieldGenerator->SetOutputSpacing(   image->GetSpacing() );
    fieldGenerator->SetOutputDirection( image->GetDirection() );
    //fieldGenerator->SetOutputSize(    image->GetRequestedRegion().GetSize() );
    //fieldGenerator->SetOutputIndex(   image->GetRequestedRegion().GetIndex() );

    // Update the field generator
    try
    {
        fieldGenerator->Update();
    }
    catch( itk::ExceptionObject& )
    {
        throw std::runtime_error( "Could not generate a displacement field from a linear transformation." );
    }

    // Gets the displacement field (image)
    typename FieldContainerType::Pointer field = fieldGenerator->GetOutput();
    field->DisconnectPipeline();

    // Create the displacement field (transformation)
    typename FieldTransformType::Pointer transform = FieldTransformType::New();
    transform->SetDisplacementField( static_cast<typename FieldContainerType::ConstPointer>( field.GetPointer() ) );
    return transform;
}


/*
template< class TTransformScalarType, class TImage >
typename itk::StationaryVelocityFieldTransform< TTransformScalarType, TImage::ImageDimension >::Pointer
linearToStationaryVelocityFieldTransformation( TImage * image, itk::Transform< TTransformScalarType, TImage::ImageDimension, TImage::ImageDimension > * linearTransform )
{
    // Type definition
    typedef  itk::StationaryVelocityFieldTransform< TTransformScalarType, TImage::ImageDimension >    FieldTransformType;
    typedef  typename  FieldTransformType::VelocityFieldType                                FieldContainerType;
    typedef  itk::TransformToVelocityFieldSource<FieldContainerType, TTransformScalarType>  GeneratorType;

    // Create a field generator
    typename GeneratorType::Pointer fieldGenerator = GeneratorType::New();
    fieldGenerator->SetTransform(                 linearTransform );
    fieldGenerator->SetOutputParametersFromImage( image );

    // Update the field generator
    try
    {
        fieldGenerator->Update();
    }
    catch( itk::ExceptionObject& err )
    {
        throw std::runtime_error( "Could not generate a velocity field from a linear transformation." );
    }

    // Gets the displacement field (image)
    typename FieldContainerType::Pointer field = fieldGenerator->GetOutput();
    field->DisconnectPipeline();

    // Create the displacement field (transformation)
    typename FieldTransformType::Pointer transform = FieldTransformType::New();
    transform->SetVelocityField( static_cast<typename FieldContainerType::ConstPointer>( field.GetPointer() ) );
    return transform;
}
*/


template< class TTransformScalarType, int Dimension >
void writeLinearTransformation(
        itk::Transform< TTransformScalarType, Dimension, Dimension > * transform,
        std::string fileName )
{
    typedef itk::TransformFileWriter TrasformWriterType;
    typename TrasformWriterType::Pointer transformWriter = TrasformWriterType::New();
    transformWriter->SetFileName( fileName );
    transformWriter->SetInput( transform );
    transformWriter->Update();
}



template< class TTransformScalarType, int Dimension >
void writeDisplacementFieldTransformation(
        itk::Transform< TTransformScalarType, Dimension, Dimension > * field,
        std::string fileName )
{
    // Cast the transformation into a displacement field
    typedef  itk::DisplacementFieldTransform< TTransformScalarType, Dimension >  FieldTransformType;
    FieldTransformType * transform = dynamic_cast<FieldTransformType *>(field);

    // Write the output field
    typedef itk::Image< itk::Vector< TTransformScalarType, Dimension >, Dimension >  FieldContainerType;
    typedef itk::ImageFileWriter< FieldContainerType > FieldWriterType;
    typename FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
    fieldWriter->SetInput(    transform->GetDisplacementField() );
    fieldWriter->SetFileName( fileName );
    fieldWriter->Update();
}


/*
template< class TTransformScalarType, int Dimension >
void writeStationaryVelocityFieldTransformation(
        itk::Transform< TTransformScalarType, Dimension, Dimension > * field,
        std::string fileName )
{
    // Cast the transformation into a displacement field
    typedef  itk::StationaryVelocityFieldTransform< TTransformScalarType, Dimension >  FieldTransformType;
    FieldTransformType * transform = dynamic_cast<FieldTransformType *>(field);

    // Write the output field
    typedef  itk::Image< itk::Vector< TTransformScalarType, Dimension >, Dimension >  FieldContainerType;
    typedef itk::ImageFileWriter< FieldContainerType > FieldWriterType;
    typename FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
    fieldWriter->SetInput(    transform->GetVelocityField() );
    fieldWriter->SetFileName( fileName );
    fieldWriter->Update();
}
*/


template< class TFixedImage, class TMovingImage, class TTransformScalarType >
void writeImage( itk::Transform< TTransformScalarType, TFixedImage::ImageDimension, TFixedImage::ImageDimension > * transform,
                 TFixedImage * fixedImage,
                 TMovingImage * movingImage,
                 std::string fileName )
{
    // Create and initialize the resample image filter
    typedef itk::ResampleImageFilter< TMovingImage, TMovingImage, TTransformScalarType > ResampleFilterType;
    typename ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    resampler->SetInput(             movingImage );
    resampler->SetTransform(         transform );
    resampler->SetSize(              fixedImage->GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputOrigin(      fixedImage->GetOrigin() );
    resampler->SetOutputSpacing(     fixedImage->GetSpacing() );
    resampler->SetOutputDirection(   fixedImage->GetDirection() );
    resampler->SetDefaultPixelValue( 0 );

    // Write the output image
    typedef itk::ImageFileWriter< TMovingImage > ImageWriterType;
    typename ImageWriterType::Pointer imageWriter = ImageWriterType::New();
    imageWriter->SetFileName( fileName );
    imageWriter->SetInput( resampler->GetOutput() );
    imageWriter->Update();
}



inline int atoi_check( const char * str )
{
    char *endptr;
    long val= strtol(str, &endptr, 0);

    // Check for various possible errors
    if ( (errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || val>=INT_MAX || val<=INT_MIN )
    {
        std::cout<<std::endl;
        std::cout<<"Cannot parse integer. Out of bound."<<std::endl;
        exit( EXIT_FAILURE );
    }

    if (endptr == str || *endptr!='\0')
    {
        std::cout<<std::endl;
        std::cout<<"Cannot parse integer. Contains non-digits or is empty."<<std::endl;
        exit( EXIT_FAILURE );
    }

    return val;
}



template< typename T >
std::vector<T> StringToVector( const std::string & str)
{
    std::vector<T> vect;

    std::string::size_type crosspos = str.find('x',0);

    if (crosspos == std::string::npos)
    {
        // only one uint
        vect.push_back( static_cast<T>( atoi_check(str.c_str()) ));
        return vect;
    }

    // first uint
    vect.push_back( static_cast<T>(atoi_check( (str.substr(0,crosspos)).c_str() ) ) );

    while(true)
    {
        std::string::size_type crossposfrom = crosspos;
        crosspos =  str.find('x',crossposfrom+1);

        if (crosspos == std::string::npos)
        {
            vect.push_back( static_cast<T>( atoi_check( (str.substr(crossposfrom+1,str.length()-crossposfrom-1)).c_str() ) ) );
            return vect;
        }

        vect.push_back( static_cast<T>( atoi_check( (str.substr(crossposfrom+1,crosspos-crossposfrom-1)).c_str() ) ) );
    }
}



template< typename T>
std::string VectorToString( std::vector<T> vector )
{
    unsigned int       size = vector.size();
    std::ostringstream oss;
    oss << "[ ";
    if (size>0)
        for (unsigned int i=0; i<size-1; i++)
            oss << vector.at(i) << ", ";
    oss << vector[size-1] << " ]";
    return oss.str();
}



inline std::string BooleanToString( bool value )
{
    return  (std::string)(value?"true":"false");
}



} // End of namespace


#endif // _RPI_COMMON_TOOLS_CXX_
