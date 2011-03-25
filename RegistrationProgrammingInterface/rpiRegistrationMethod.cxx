#ifndef _RPI_REGISTRATION_METHOD_CXX_
#define _RPI_REGISTRATION_METHOD_CXX_

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "rpiRegistrationMethod.hxx"


// Namespace RPI : Registration Programming Interface
namespace rpi
{


template < class TFixedImage, class TMovingImage, class TTransformScalarType >
RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::RegistrationMethod(void)
{
    if ( (int)TFixedImage::ImageDimension != (int)TMovingImage::ImageDimension )
        throw std::runtime_error("Fixed and moving images must have same dimension.");
}


template < class TFixedImage, class TMovingImage, class TTransformScalarType >
RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::~RegistrationMethod(void)
{
    // Do nothing
}


template < class TFixedImage, class TMovingImage, class TTransformScalarType >
typename RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::FixedImageConstPointerType
RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::GetFixedImage(void) const
{
    return this->m_fixedImage;
}


template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::SetFixedImage(const FixedImageType * image)
{
    this->m_fixedImage = image;
}


template < class TFixedImage, class TMovingImage, class TTransformScalarType >
typename RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::MovingImageConstPointerType
RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::GetMovingImage(void) const
{
    return m_movingImage;
}


template < class TFixedImage, class TMovingImage, class TTransformScalarType >
void
RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::SetMovingImage(const MovingImageType * image)
{
    this->m_movingImage = image;
}


template < class TFixedImage, class TMovingImage, class TTransformScalarType >
typename RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::TransformPointerType
RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >::GetTransformation(void) const
{
    return m_transform;
}


} // End of namespace


#endif // _RPI_REGISTRATION_METHOD_CXX_
