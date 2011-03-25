#ifndef _RPI_COMMON_TOOLS_HXX_
#define _RPI_COMMON_TOOLS_HXX_


#include <iostream>
#include <sstream>

#include <itkImageIOBase.h>
#include <itkDisplacementFieldTransform.h>
#include <itkStationaryVelocityFieldTransform.h>
#include <itkEuler3DTransform.h>

#include <rpiRegistrationMethod.hxx>


// Namespace RPI : Registration Programming Interface
namespace rpi
{


/**
 * Reads image information.
 * @param   fileName  name of the image to read
 * @return  image information
 */
itk::ImageIOBase::Pointer readImageInformation( std::string fileName );


/**
 * Reads image.
 * @param   fileName  name of the image to read
 * @return  image
 */
template< class TImage >
typename TImage::Pointer  readImage( std::string fileName );


/**
 * Read an ITK transformation from an input file.
 * @param   filename  input file name
 * @return  transformation
 */
template< class TTransformScalarType>
typename itk::Transform< TTransformScalarType, 3, 3 >::Pointer
read3DTransformation( std::string fileName );


/**
 * Read an ITK Euler3D transformation from an input file.
 * @param   filename  input file name
 * @return  Euler3D transformation
 */
template< class TTransformScalarType>
typename itk::Euler3DTransform< TTransformScalarType >::Pointer
readEuler3DTransformation( std::string fileName );


/**
 * Read an ITK disaplcement field from an input image.
 * @param   filename  input image name
 * @return  displacement field
 */
template< class TTransformScalarType, int TDimension >
typename itk::DisplacementFieldTransform< TTransformScalarType, TDimension >::Pointer
readDisplacementField( std::string fileName );


/**
 * Read an ITK stationary velocity field from an input image.
 * @param   filename  input image name
 * @return  stationary velocity field
 */
template< class TTransformScalarType, int TDimension >
typename itk::StationaryVelocityFieldTransform< TTransformScalarType, TDimension >::Pointer
readStationaryVelocityField( std::string fileName );


/**
 * Converts a linear transformation into a displacement field transformation.
 * The geometry of the displacement field is taken from the input image.
 * @param   image            image
 * @param   linearTransform  linear transformation
 * @return  displacement field transformation
 */
template< class TTransformScalarType, class TImage >
typename itk::DisplacementFieldTransform< TTransformScalarType, TImage::ImageDimension >::Pointer
linearToDisplacementFieldTransformation(
        TImage * image,
        itk::Transform< TTransformScalarType, TImage::ImageDimension, TImage::ImageDimension > * linearTransform );


/**
 * Converts a linear transformation into a stationary velocity field transformation.
 * The geometry of the displacement field is taken from the input image.
 * @param   image            image
 * @param   linearTransform  linear transformation
 * @return  stationary velocity field transformation
 */
template< class TTransformScalarType, class TImage >
typename itk::StationaryVelocityFieldTransform< TTransformScalarType, TImage::ImageDimension >::Pointer
linearToStationaryVelocityFieldTransformation(
        TImage * image,
        itk::Transform< TTransformScalarType, TImage::ImageDimension, TImage::ImageDimension > * linearTransform );


/**
 * Gets and writes the output transformation into an output file.
 * @param  transformation  registration object
 * @param  fileName        name of the output file
 */
template< class TTransformScalarType, int Dimension >
void writeLinearTransformation(
        itk::Transform< TTransformScalarType, Dimension, Dimension > * transform,
        std::string fileName );


/**
 * Writes a displacement field into an output file.
 * @param  field     displacement field
 * @param  fileName  name of the output file
 */
template< class TTransformScalarType, int Dimension >
void writeDisplacementFieldTransformation(
        itk::Transform< TTransformScalarType, Dimension, Dimension > * field,
        std::string fileName );


/**
 * Writes a stationary velocity field into an output file.
 * @param  field     stationary velocity field
 * @param  fileName  name of the output file
 */
template< class TTransformScalarType, int Dimension >
void writeStationaryVelocityFieldTransformation(
        itk::Transform< TTransformScalarType, Dimension, Dimension > * field,
        std::string fileName );


/**
 * Resamples the moving image in the geometry of the fixed image and saves the resulting image into an output file.
 * @param  transform    transformation
 * @param  fixedImage   fixed image
 * @param  movingImage  moving image
 * @param  fileName     name of the output file
 */
template< class TFixedImage, class TMovingImage, class TTransformScalarType >
void writeImage(
        itk::Transform< TTransformScalarType, TFixedImage::ImageDimension, TFixedImage::ImageDimension > * transform,
        TFixedImage * fixedImage,
        TMovingImage * movingImage,
        std::string fileName );


/**
 * Parse a string and generate a vector of unsigned int.
 * @param   str  string to parse
 * @return  vector of unsigned int
 */
template< typename T >
std::vector<T> StringToVector( const std::string & str );


/**
 * Displays a std::vector as a string.
 * @param   vector  the std::vector to be displayed
 * @return  std::vector as a string
 */
template< typename T>
std::string VectorToString( std::vector<T> vector );

/**
 * Displays a boolean as a string.
 * @param   value  boolean value
 * @return  boolean as a string
 */
std::string BooleanToString( bool value );


} // End of namespace


#include "rpiCommonTools.cxx"


#endif // _RPI_COMMON_TOOLS_HXX_
