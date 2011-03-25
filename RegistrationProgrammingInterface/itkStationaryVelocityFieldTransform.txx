#ifndef _itkStationaryVelocityFieldTransform_cxx_
#define _itkStationaryVelocityFieldTransform_cxx_

#include <itkNeighborhoodAlgorithm.h>
#include <itkImageRegionIterator.h>
#include <vnl/vnl_det.h>

#include "itkStationaryVelocityFieldTransform.h"


namespace itk
{


// Constructor with default arguments
template <class TScalarType, unsigned int NDimensions>
StationaryVelocityFieldTransform<TScalarType, NDimensions>
::StationaryVelocityFieldTransform():
 Superclass( SpaceDimension, ParametersDimension )
{
    m_VelocityField       = 0;
    m_InterpolateFunction = InterpolateFunctionType::New();
}

// Print self
template<class TScalarType, unsigned int NDimensions>
void
StationaryVelocityFieldTransform<TScalarType, NDimensions>::
PrintSelf(std::ostream &os, Indent indent) const
{
    Superclass::PrintSelf(os,indent);
}


template<class TScalarType, unsigned int NDimensions>
void
StationaryVelocityFieldTransform<TScalarType, NDimensions>::
SetVelocityField(VelocityFieldConstPointerType field)
{
    m_VelocityField = field;
    m_InterpolateFunction->SetInputImage( m_VelocityField );

    itk::Vector<double, NDimensions> spacing = m_VelocityField->GetSpacing();
    for (unsigned int i=0; i<NDimensions; i++)
        m_DerivativeWeights[i] = (double)(1.0/spacing[i]);

    this->Modified();
}


// Transform a point
template<class TScalarType, unsigned int NDimensions>
typename StationaryVelocityFieldTransform<TScalarType, NDimensions>::OutputPointType
StationaryVelocityFieldTransform<TScalarType, NDimensions>::
TransformPoint(const InputPointType &point) const 
{
    itkExceptionMacro( "TransformPoint method not implemented yet." );
    //throw std::runtime_error( "TransformPoint() method not implemented yet." );
    //return point;
}


// Transform a vector
template<class TScalarType, unsigned int NDimensions>
typename StationaryVelocityFieldTransform<TScalarType, NDimensions>::OutputVectorType
StationaryVelocityFieldTransform<TScalarType, NDimensions>::
TransformVector(const InputVectorType &vect) const 
{
    itkExceptionMacro ("cannot transform vector !");
}


// Transform a vnl_vector_fixed
template<class TScalarType, unsigned int NDimensions>
typename StationaryVelocityFieldTransform<TScalarType, NDimensions>::OutputVnlVectorType
StationaryVelocityFieldTransform<TScalarType, NDimensions>::
TransformVector(const InputVnlVectorType &vect) const 
{
    itkExceptionMacro ("cannot transform vnl_vector !");
}


// Transform a CovariantVector
template<class TScalarType, unsigned int NDimensions>
typename StationaryVelocityFieldTransform<TScalarType, NDimensions>::OutputCovariantVectorType
StationaryVelocityFieldTransform<TScalarType, NDimensions>::
TransformCovariantVector(const InputCovariantVectorType &vect) const 
{  
    itkExceptionMacro ("cannot transform covariant vector !");
}
  
// Compute the Jacobian determinant in one position 
template<class TScalarType, unsigned int NDimensions>
typename StationaryVelocityFieldTransform<TScalarType, NDimensions>::ScalarType
StationaryVelocityFieldTransform< TScalarType,  NDimensions >::
GetJacobianDeterminantWithRespectToCoordinates( const InputPointType & point) const
{ 
    itkExceptionMacro( "GetJacobianDeterminantWithRespectToCoordinates method not implemented yet." );
    //throw std::runtime_error( "GetJacobianDeterminantWithRespectToCoordinates() method not implemented yet." );
    //return static_cast<ScalarType>(vnl_det(this->GetJacobianWithRespectToCoordinates (point)));
}

// Compute the Jacobian in one position 
template<class TScalarType, unsigned int NDimensions>
vnl_matrix_fixed<double,NDimensions,NDimensions>
StationaryVelocityFieldTransform<TScalarType, NDimensions>::
GetJacobianWithRespectToCoordinates( const InputPointType & point) const
{

    itkExceptionMacro( "GetJacobianWithRespectToCoordinates method not implemented yet." );
    /*std::cout << "WARNING : The GetJacobianWithRespectToCoordinates() method of the StationaryVelocityFieldTransform object is wrong!!! " << std::endl;
    unsigned int i, j;
    vnl_matrix_fixed<double,NDimensions,NDimensions> J;

    if (!m_VelocityField)
    {
        itkExceptionMacro ("No displacement field, cannot compute jacobian !");
    }

    double weight;
    itk::Vector<double, NDimensions> spacing = m_VelocityField->GetSpacing();

    InputPointType point_prev, point_next;

    for (i = 0; i < NDimensions; ++i)
    {
        point_prev[i] = static_cast<double>(point[i]) - spacing[i];
        point_next[i] = static_cast<double>(point[i]) + spacing[i];
    }

    typename InterpolateFunctionType::OutputType vector = m_InterpolateFunction->Evaluate (point);
    typename InterpolateFunctionType::OutputType vector_prev = m_InterpolateFunction->Evaluate (point_prev);
    typename InterpolateFunctionType::OutputType vector_next = m_InterpolateFunction->Evaluate (point_next);

    for (i = 0; i < NDimensions; ++i)
    {
        weight = 0.5*m_DerivativeWeights[i];

        for (j = 0; j < NDimensions; ++j)
        {

            J[i][j]=weight*(static_cast<double>(vector_next[j])
                            -static_cast<double>(vector_prev[j]));
        }

        // add one on the diagonal to consider the warp
        // and not only the deformation field
        J[i][i] += 1.0;
    }

    return J;*/
}


  
} // namespace

#endif
