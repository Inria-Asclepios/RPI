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
#include <itkMatrixOffsetTransformBase.h>
#include <itkAffineTransform.h>
#include <itkEuler2DTransform.h>
#include <itkEuler3DTransform.h>
#include <itkTransformFileReader.h>
#include <itkTransformFileWriter.h>
#include <itkTransformToDeformationFieldSource.h>
#include <itkTransformToVelocityFieldSource.h>

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
typename TImage::Pointer readImage( std::string fileName )
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



/**
 * Local function. Reads a transform from a text file and returns a TransformBase::Pointer object.
 * @param  fileName path to the input transformation file
 * @return transformation read as TransformBase::Pointer object
 */
typename itk::TransformBase::Pointer
readTransformBase( std::string fileName )
{

    // Type definition
    typedef  itk::TransformFileReader                TransformReaderType;
    typedef  TransformReaderType::TransformListType  TransformListType;

    // Define transformation reader
    TransformReaderType::Pointer reader = TransformReaderType::New();
    reader->SetFileName( fileName );

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

    // Return the pointer on the transformation
    return list->front();

}



/**
 * Local function. Cast the scalar type of an input affine transformation.
 * @param  input input transformation
 * @return transformation casted
 */
template<class TInputScalarType, class TOutputScalarType>
typename itk::AffineTransform<TOutputScalarType,3>::Pointer
castAffineTransform(itk::AffineTransform<TInputScalarType,3> * input)
{
    // Typedef for input transformation
    typedef  itk::AffineTransform<TInputScalarType,3>      InputTransformType;
    typedef  typename InputTransformType::ParametersType   InputParameterType;
    typedef  typename InputTransformType::InputPointType   InputCenterType;

    // Typedef for output transformation
    typedef  itk::AffineTransform<TOutputScalarType,3>     OutputTransformType;
    typedef  typename OutputTransformType::ParametersType  OutputParameterType;
    typedef  typename OutputTransformType::InputPointType  OutputCenterType;

    // Get input parameters and center
    InputParameterType iP = input->GetParameters();
    InputCenterType    iC = input->GetCenter();

    // Create output parameters and center
    OutputParameterType oP(InputTransformType::ParametersDimension);
    OutputCenterType    oC;
    for (int i=0; i<InputTransformType::ParametersDimension; i++)
        oP[i] = iP[i];
    for (int i=0; i<3; i++)
        oC[i] = iC[i];

    // Create the output transformation
    typename OutputTransformType::Pointer output = OutputTransformType::New();
    output->SetParameters( oP );
    output->SetCenter(     oC );

    return output;
}



/**
 * Local function. Cast the scalar type of an input Euler3D transformation.
 * @param  input input transformation
 * @return transformation casted
 */
template<class TInputScalarType, class TOutputScalarType>
typename itk::Euler3DTransform<TOutputScalarType>::Pointer
castEuler3DTransform(itk::Euler3DTransform<TInputScalarType> * input)
{
    // Typedef for input transformation
    typedef  itk::Euler3DTransform<TInputScalarType>       InputTransformType;
    typedef  typename InputTransformType::ParametersType   InputParameterType;
    typedef  typename InputTransformType::InputPointType   InputCenterType;

    // Typedef for output transformation
    typedef  itk::Euler3DTransform<TOutputScalarType>      OutputTransformType;
    typedef  typename OutputTransformType::ParametersType  OutputParameterType;
    typedef  typename OutputTransformType::InputPointType  OutputCenterType;

    // Get input parameters and center
    InputParameterType iP = input->GetParameters();
    InputCenterType    iC = input->GetCenter();

    // Create output parameters and center
    OutputParameterType oP(InputTransformType::ParametersDimension);
    OutputCenterType    oC;
    for (int i=0; i<InputTransformType::ParametersDimension; i++)
        oP[i] = iP[i];
    for (int i=0; i<3; i++)
        oC[i] = iC[i];

    // Create the output transformation
    typename OutputTransformType::Pointer output = OutputTransformType::New();
    output->Register();
    output->SetParameters( oP );
    output->SetCenter(     oC );

    return output;
}



template< class TTransformScalarType>
typename itk::Euler3DTransform< TTransformScalarType >::Pointer
readEuler3DTransformation( std::string fileName )
{

    // Type definition
    typedef  itk::TransformBase               TransformBaseType;
    typedef  itk::Euler3DTransform< float >   Euler3DFloat;
    typedef  itk::Euler3DTransform< double >  Euler3DDouble;

    // Read the TransformBase object
    TransformBaseType * transform = readTransformBase(fileName).GetPointer();

    // Try to cast as Euler3DFloat
    Euler3DFloat *euler3DFloat = dynamic_cast<Euler3DFloat*>(transform);
    if(euler3DFloat != 0)
        return castEuler3DTransform<float,TTransformScalarType>(euler3DFloat);

    // Try to cast as Euler3DDouble
    Euler3DDouble *euler3DDouble = dynamic_cast<Euler3DDouble*>(transform);
    if(euler3DDouble != 0)
        return castEuler3DTransform<double,TTransformScalarType>(euler3DDouble);

    // Otherwise, throw an exception
    throw std::runtime_error( "Transformation type not supported." );
    return 0;
}



template< class TTransformScalarType, int TDimension>
typename itk::AffineTransform< TTransformScalarType, TDimension >::Pointer
readAffineTransformation( std::string fileName )
{

    // Type definition
    typedef  itk::TransformBase                 TransformBaseType;
    typedef  itk::AffineTransform< float, 3 >   AffineFloat;
    typedef  itk::AffineTransform< double, 3 >  AffineDouble;

    // Read the TransformBase object
    TransformBaseType * transform = readTransformBase(fileName).GetPointer();

    // Try to cast as AffineFloat
    AffineFloat *affineFloat = dynamic_cast<AffineFloat*>(transform);
    if(affineFloat != 0)
        return castAffineTransform<float,TTransformScalarType>(affineFloat);

    // Try to cast as AffineDouble
    AffineDouble *affineDouble = dynamic_cast<AffineDouble*>(transform);
    if(affineDouble != 0)
        return castAffineTransform<double,TTransformScalarType>(affineDouble);

    // Otherwise, throw an exception
    throw std::runtime_error( "Transformation type not supported." );
    return 0;
}



template<class TTransformScalarType>
typename itk::Transform< TTransformScalarType, 3, 3 >::Pointer
readLinearTransformation( std::string fileName )
{

    // Type definition
    typedef  itk::TransformBase                 TransformBaseType;
    typedef  itk::Euler3DTransform< float >     Euler3DFloat;
    typedef  itk::Euler3DTransform< double >    Euler3DDouble;
    typedef  itk::AffineTransform< float, 3 >   AffineFloat;
    typedef  itk::AffineTransform< double, 3 >  AffineDouble;
    typedef  itk::MatrixOffsetTransformBase< float, 3, 3 >   MatrixOffsetFloat;
    typedef  itk::MatrixOffsetTransformBase< double, 3, 3 >  MatrixOffsetDouble;

    // Read the TransformBase object
    TransformBaseType * transform = readTransformBase(fileName).GetPointer();

    // Note : In the following, we return the pointer (using GetPointer) instead of the itk::Pointer
    // to allow the automatic cast of the returned transform into a itk::Transform object.

    // Try to cast as Euler3DFloat
    Euler3DFloat *euler3DFloat = dynamic_cast<Euler3DFloat*>(transform);
    if(euler3DFloat != 0)
        return castEuler3DTransform<float,TTransformScalarType>(euler3DFloat).GetPointer();

    // Try to cast as Euler3DDouble
    Euler3DDouble *euler3DDouble = dynamic_cast<Euler3DDouble*>(transform);
    if(euler3DDouble != 0)
        return castEuler3DTransform<double,TTransformScalarType>(euler3DDouble).GetPointer();

    // Try to cast as AffineFloat
    AffineFloat *affineFloat = dynamic_cast<AffineFloat*>(transform);
    if(affineFloat != 0)
        return castAffineTransform<float,TTransformScalarType>(affineFloat).GetPointer();

    // Try to cast as AffineDouble
    AffineDouble *affineDouble = dynamic_cast<AffineDouble*>(transform);
    if(affineDouble != 0)
        return castAffineTransform<double,TTransformScalarType>(affineDouble).GetPointer();

    // Otherwise, throw an exception
    throw std::runtime_error( "Transformation type not supported." );
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



template< class TTransformScalarType, int TDimension >
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



template< class TLinearScalarType, class TFieldScalarType, class TImage >
typename itk::DisplacementFieldTransform< TFieldScalarType, TImage::ImageDimension >::Pointer
linearToDisplacementFieldTransformation(
        TImage * image,
        itk::Transform< TLinearScalarType, TImage::ImageDimension, TImage::ImageDimension > * transform )
{
    typedef  itk::DisplacementFieldTransform< TFieldScalarType, TImage::ImageDimension >
            FieldTransformType;

    typedef  typename FieldTransformType::DisplacementFieldType
            FieldContainerType;

    typedef  itk::TransformToDeformationFieldSource< FieldContainerType, TLinearScalarType >
            GeneratorType;

    // Create a field generator
    typename GeneratorType::Pointer fieldGenerator = GeneratorType::New();
    fieldGenerator->SetTransform(                 transform );
    fieldGenerator->SetOutputParametersFromImage( image );

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
    typename FieldContainerType::Pointer container = fieldGenerator->GetOutput();
    container->DisconnectPipeline();

    // Create the displacement field (transformation)
    typename FieldTransformType::Pointer field = FieldTransformType::New();
    field->SetDisplacementField( static_cast<typename FieldContainerType::ConstPointer>( container.GetPointer() ) );
    return field;
}



template< class TLinearScalarType, class TFieldScalarType, class TImage >
typename itk::StationaryVelocityFieldTransform< TFieldScalarType, TImage::ImageDimension >::Pointer
linearToStationaryVelocityFieldTransformation(
        TImage * image,
        itk::Transform< TLinearScalarType, TImage::ImageDimension, TImage::ImageDimension > * transform )
{
    typedef  itk::StationaryVelocityFieldTransform< TFieldScalarType, TImage::ImageDimension >
            FieldTransformType;

    typedef  typename FieldTransformType::VelocityFieldType
            FieldContainerType;

    typedef  itk::TransformToVelocityFieldSource< FieldContainerType, TLinearScalarType >
            GeneratorType;

    // Create a field generator
    typename GeneratorType::Pointer fieldGenerator = GeneratorType::New();
    fieldGenerator->SetTransform(                 transform );
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
    typename FieldContainerType::Pointer container = fieldGenerator->GetOutput();
    container->DisconnectPipeline();

    // Create the displacement field (transformation)
    typename FieldTransformType::Pointer field = FieldTransformType::New();
    field->SetVelocityField( static_cast<typename FieldContainerType::ConstPointer>( container.GetPointer() ) );
    return field;
}



template< class TTransformScalarType, int TDimension >
void writeLinearTransformation(
        itk::Transform< TTransformScalarType, TDimension, TDimension > * transform,
        std::string fileName )
{
    typedef itk::TransformFileWriter TrasformWriterType;
    typename TrasformWriterType::Pointer transformWriter = TrasformWriterType::New();
    transformWriter->SetFileName( fileName );
    transformWriter->SetInput( transform );
    transformWriter->Update();
}



template< class TTransformScalarType, int TDimension >
void writeDisplacementFieldTransformation(
        itk::Transform< TTransformScalarType, TDimension, TDimension > * field,
        std::string fileName )
{
    // Cast the transformation into a displacement field
    typedef  itk::DisplacementFieldTransform< TTransformScalarType, TDimension >  FieldTransformType;
    FieldTransformType * transform = dynamic_cast<FieldTransformType *>(field);

    // Write the output field
    typedef itk::Image< itk::Vector< TTransformScalarType, TDimension >, TDimension >  FieldContainerType;
    typedef itk::ImageFileWriter< FieldContainerType > FieldWriterType;
    typename FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
    fieldWriter->SetInput(    transform->GetDisplacementField() );
    fieldWriter->SetFileName( fileName );
    fieldWriter->Update();
}



template< class TTransformScalarType, int TDimension >
void writeStationaryVelocityFieldTransformation(
        itk::Transform< TTransformScalarType, TDimension, TDimension > * field,
        std::string fileName )
{
    // Cast the transformation into a displacement field
    typedef  itk::StationaryVelocityFieldTransform< TTransformScalarType, TDimension >  FieldTransformType;
    FieldTransformType * transform = dynamic_cast<FieldTransformType *>(field);

    // Write the output field
    typedef  itk::Image< itk::Vector< TTransformScalarType, TDimension >, TDimension >  FieldContainerType;
    typedef itk::ImageFileWriter< FieldContainerType > FieldWriterType;
    typename FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
    fieldWriter->SetInput(    transform->GetVelocityField() );
    fieldWriter->SetFileName( fileName );
    fieldWriter->Update();
}



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
    unsigned int size = vector.size();
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
    return (std::string)(value?"true":"false");
}



} // End of namespace


#endif // _RPI_COMMON_TOOLS_CXX_
