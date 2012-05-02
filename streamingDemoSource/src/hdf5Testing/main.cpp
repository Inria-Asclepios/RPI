
#include <itkImageToVTKImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImage.h>
#include <itkImageFileWriter.h>

#include <vtkImageData.h>

#include <iostream>
#include <stdexcept>
typedef itk::Image<unsigned short, 3> ImageType;

#include <hdf5.h>


int writeExample( ImageType::ConstPointer image, const std::string & filename, const std::string & dataName )
{
    const ImageType::RegionType region = image->GetLargestPossibleRegion();

    herr_t          status;
    const unsigned int ndims = 3;
    hsize_t         dims[ndims],
                    chunk[ndims];
    unsigned int    filter_info;

    for ( int i(0); i<ndims; ++i ) {
        dims[i] = region.GetSize(i);
    }

    /*
     * Check if gzip compression is available and can be used for both
     * compression and decompression.  Normally we do not perform error
     * checking in these examples for the sake of clarity, but in this
     * case we will make an exception because this filter is an
     * optional part of the hdf5 library.
     */
    htri_t avail = H5Zfilter_avail(H5Z_FILTER_DEFLATE);
    if (!avail) {
        printf ("gzip filter not available.\n");
        return 1;
    }
    status = H5Zget_filter_info (H5Z_FILTER_DEFLATE, &filter_info);
    if ( !(filter_info & H5Z_FILTER_CONFIG_ENCODE_ENABLED) ||
                !(filter_info & H5Z_FILTER_CONFIG_DECODE_ENABLED) ) {
        printf ("gzip filter not available for encoding and decoding.\n");
        return 1;
    }


    /*
     * Create a new file using the default properties.
     */
    hid_t outfileId = H5Fcreate (filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    /*
     * Create dataspace.  Setting maximum size to NULL sets the maximum
     * size to be the current size.
     */
    hid_t space = H5Screate_simple (ndims, dims, NULL);

    /*
     * Create the dataset creation property list, add the gzip
     * compression filter and set the chunk size.
     */
    chunk[0] = 16;
    chunk[1] = 16;
    chunk[2] = 16;
    hid_t dcpl = H5Pcreate (H5P_DATASET_CREATE);
    status = H5Pset_deflate (dcpl, 7);
    status = H5Pset_chunk (dcpl, ndims, chunk);

    hid_t dataType;

    if ( typeid( ImageType::PixelType ) == typeid( short ) ) {
        dataType = H5T_NATIVE_SHORT;
    } else if ( typeid( ImageType::PixelType ) == typeid( unsigned short ) ) {
        dataType = H5T_NATIVE_USHORT;
    } else {
        throw( std::runtime_error("Unknown data type for image") );
    }
    /*
     * Create the dataset.
     */
    hid_t dset = H5Dcreate (outfileId, dataName.c_str(), dataType, space, H5P_DEFAULT, dcpl, H5P_DEFAULT);

    /*
     * Write the data to the dataset.
     */
    status = H5Dwrite (dset, dataType, H5S_ALL, H5S_ALL, H5P_DEFAULT, image->GetBufferPointer() ) ;

    /*
     * Close and release resources.
     */
    status = H5Pclose (dcpl);
    status = H5Dclose (dset);
    status = H5Sclose (space);
    status = H5Fclose (outfileId);

    return 0;
}


ImageType::Pointer readExample(const std::string & filename, const std::string & dataName)
{
    herr_t          status;
    /*
     * Open file and dataset using the default properties.
     */
    hid_t file = H5Fopen (filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dset = H5Dopen (file, dataName.c_str(), H5P_DEFAULT);

    /*
     * Retrieve dataset creation property list.
     */
    hid_t dcpl = H5Dget_create_plist (dset);

    /*
     * Retrieve and print the filter type.  Here we only retrieve the
     * first filter because we know that we only added one filter.
     */
    size_t nelmts = 0;
    unsigned int    filter_info;
    unsigned int    flags;

    H5Z_filter_t filter_type = H5Pget_filter (dcpl, 0, &flags, &nelmts, NULL, 0, NULL, &filter_info);
    printf ("Filter type is: ");
    switch (filter_type) {
        case H5Z_FILTER_DEFLATE:
            printf ("H5Z_FILTER_DEFLATE\n");
            break;
        case H5Z_FILTER_SHUFFLE:
            printf ("H5Z_FILTER_SHUFFLE\n");
            break;
        case H5Z_FILTER_FLETCHER32:
            printf ("H5Z_FILTER_FLETCHER32\n");
            break;
        case H5Z_FILTER_SZIP:
            printf ("H5Z_FILTER_SZIP\n");
            break;
        case H5Z_FILTER_NBIT:
            printf ("H5Z_FILTER_NBIT\n");
            break;
        case H5Z_FILTER_SCALEOFFSET:
            printf ("H5Z_FILTER_SCALEOFFSET\n");
    }

    // DEtermine dimension of data.

    hid_t space = H5Dget_space( dset );

    const int ndims = H5Sget_simple_extent_ndims( space );

    if ( ImageType::ImageDimension != ndims ) {
        throw ( std::runtime_error("Bad dimension") );
    }

    std::vector<hsize_t> dims(ndims), maxdims(ndims);
    status = H5Sget_simple_extent_dims(space, &(dims[0]), &(maxdims[0]) );

    ImageType::Pointer ret = ImageType::New();
    ImageType::RegionType largestRegion;
    for (int i(0); i<ndims; ++i ) {
        largestRegion.SetSize(i, maxdims[i]);
    }

    ret->SetLargestPossibleRegion(largestRegion);
    ret->SetBufferedRegion(largestRegion);
    ret->Allocate();
    //TODO ret->SetSpacing(spacing);
    //TODO ret->SetDirection(direction);
    /*
     * Read the data using the default properties.
     */
    hid_t dataType;
    if (typeid( ImageType::PixelType ) == typeid( short ) ) {
        dataType = H5T_NATIVE_SHORT;
    } else {
        throw( std::runtime_error( " Bad data type " ) ) ;
    }
    status = H5Dread (dset, dataType, H5S_ALL, H5S_ALL, H5P_DEFAULT, ret->GetBufferPointer() );

    /*
     * Close and release resources.
     */
    status = H5Pclose (dcpl);
    status = H5Dclose (dset);
    status = H5Sclose (space);
    status = H5Fclose (file);

    return ret;
}

ImageType::Pointer readRaw( const std::string & filename, int xdim, int ydim, int zdim )
{
    ImageType::Pointer ret = ImageType::New();
    ImageType::RegionType region;
    region.SetSize(0,xdim);
    region.SetSize(1,ydim);
    region.SetSize(2,zdim);

    ret->SetLargestPossibleRegion( region );
    ret->SetBufferedRegion( region );
    ret->Allocate();

    std::ifstream is( filename, std::ifstream::binary );
    size_t buffSize = ret->GetOffsetTable()[ ret->GetImageDimension() ];
    is.read( reinterpret_cast<char *>(ret->GetBufferPointer()), buffSize * sizeof(ImageType::PixelType));

    return ret;
}

int main(int argc, char * argv[])
{

    const std::string inputFilename = "c:/Temp/brebix.mha";
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer Reader = ReaderType::New();
    Reader->SetFileName( inputFilename );
    ImageType::Pointer image = Reader->GetOutput();
    image->Update();

    const std::string outfile = "c:/Temp/testFile.hdf5";
    const std::string dataName = "imageData";

    const std::string infileRaw = "c:/Temp/highres400x400x100.raw";
    const std::string outfilemha = "c:/Temp/testFile.mha";
    

    image = readRaw(infileRaw, 400, 400, 100 );
    typedef itk::ImageFileWriter< ImageType > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput( image );
    writer->SetFileName(outfilemha);
    writer->Write();

    writeExample( ImageType::ConstPointer(image), outfile, dataName);

    return 0;
}

