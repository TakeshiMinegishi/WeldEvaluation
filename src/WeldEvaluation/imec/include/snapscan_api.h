/////////////////////////////////////////////////////
//
//  Date: 9 April, 2018
//
//  Authors:
//    Wouter Charle, Vincent Radelet
//
//  Copyright 2018 imec. All rights reserved.
//
////////////////////////////////////////////

#ifndef SNAPSCAN_API_H
#define SNAPSCAN_API_H

// snapscan api includes
#include "snapscan_api_lib.h"
#include "hsi_api_types.h"
#include "hsi_api_common.h"

#include <wchar.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    /*! 
    \brief Structure gathering all snapscan system properties.
    
    Information about the snapscan system. 
    Some configuration and runtime settings are thresholded by the system itself.

    \sa GetSystemProperties
    */
    typedef struct SystemProperties
    {
        int cube_width_max;                  ///< Maximum cube width. Limited by the sensor width. 
        int cube_height_max;                 ///< Maximum cube height. Limited by the scanning range and the number of pixels per band.
        int pixel_step_max;                  ///< Maximum pixel step. Limited by the number of pixels per band.
        int stage_position_px_max;           ///< Maximum stage position in pixel. Symetric around the center.
        long long memory_requirements_bytes; ///< Size of the internal buffers, in bytes.
    } SystemProperties;

    /*! 
    \brief Structure gathering all snapscan configuration parameters.
    
    Configuration parameters determine the data output size. They impact the allocation of both internal and external buffers.
    These parameters can be set only when the snapscan is stopped and will be used to correctly initialize the snapscan.

    \sa CubeDataFormat, Stop, Initialize
    */
    typedef struct ConfigurationParameters
    {
        int     bit_depth;              ///< Must be a valid option for the connected camera. See the corresponding manual.
        int     cube_width;             ///< Must be contained in the interval ]0, (SystemProperties) cube_width_max]. Number of columns in the data.
        int     cube_offset_x;          ///< Must be contained in the interval ]0, (SystemProperties) cube_width_max - cube_width]. Shifts the region of interest along the width axis.
        bool    auto_centering_x;       ///< True or false. Automatically compute the cube_offset_x to center the cube.
        int     cube_height;            ///< Must be contained in the interval ]0, (SystemProperties) cube_height_max]. Number of rows in the data.
        int     tdi_pixel_step;         ///< Must be contained in the interval ]0, (SystemProperties) pixel_step_max]. Increment of the translation stage after each frame acquisition within a cube.
        bool    discard_edge_pixels;    ///< True or false. Ignore information contained at the extremal pixels of each band to reduce crosstalk.
        int     nxn_binning_size;       ///< Must be contained in the interval ]0, min (cube_width, cube_height)]. Downsample the acquisition by binning a square neighborhood into one resulting pixel.
    } ConfigurationParameters;

    /*! 
    \brief Structure gathering all snapscan runtime parameters.

    Runtime parameters determine the acquisition characteristics. They impact the sensor readout and scanning speed.
    These parameters can always be set except when the snapscan is started and will be set on the device when starting.
    
    \sa CubeDataInfo, Pause, Start
    */
    typedef struct RuntimeParameters
    {
        double          integration_time_ms;      ///< Must be contained in the interval [0.05, 500]. Exposure time during which light is collected.
        double          pixel_blur_max;           ///< Must be contained in the interval ]0, tdi_pixel_step[. While only relevant in continuous mode, it thresholds the translation velocity to prevent motion blur lowering the data quality.
        bool            input_trigger_enabled;    ///< True or false. Enable triggering on input port from external device.
        bool            input_trigger_polarity;   ///< True or false.
        int             input_trigger_delay_ms;   ///< Must be positive.
        int             multiframe_nr_frames;     ///< Nr. of frames to average per position.
    } RuntimeParameters;

    /*! 
    \brief Structure representing a correction matrix. 
    */
    typedef struct CorrectionMatrix
    {
        char*       name;       ///< Name of the correction matrix.
        int         nr_rows;    ///< Number of rows in the correction matrix.
        int         nr_cols;    ///< Number of columns in the correction matrix.
        double*     wavelengths_nm;

        double const* const* pp_data;    ///< 2D data pointer to the matrix data, ordered row-major (i.e., \code{.c}pp_data[row_idx][col_idx]\endcode). 

        void*       p_reserved; ///< Reserved pointer. Do not use.
    } CorrectionMatrix;

    /*! 
    \brief Structure representing a wavelength region.
    */
    typedef struct SpectralRegionOfInterest
    {
        double wavelength_start_nm; ///< Start wavelength of the region, in nanometers.
        double wavelength_end_nm;   ///< End wavelength of the region, in nanometers.
    } SpectralRegionOfInterest;

    /*!
    \brief Function to retrieve the internal version numbers as specified inside the library.

    \param[out] o_p_major pointer to an integer to contain the major version number.
    \param[out] o_p_minor pointer to an integer to contain the minor version number.
    \param[out] o_p_patch pointer to an integer to contain the patch version number.
    \param[out] o_p_build pointer to an integer to contain the build version number.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    SNAPSCAN_API HSI_RETURN GetAPIVersion (int* o_p_major, int* o_p_minor, int* o_p_patch, int* o_p_build);

    /*! 
    \brief Opens the connection to a snapscan device and creates a device handle.

    \param[out] o_p_device_handle Pointer to a snapscan device handle. Will be set if opening the device succeeded.
    \param[in]  i_system_file_path Path to the snapscan system file. Can be either a relative or absolute path.
    \param[in]  i_dummy_api Opens a connection to a dummy object. Can be used to test the integration of the API without physical device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_FILE_NOT_FOUND The file snapscan system specified by i_system_file_path could not be found.
    \retval HSI_ARGUMENT_INVALID The file snapscan system specified by i_system_file_path is invalid.
    \retval HSI_CALIBRATION_FILE_NOT_FOUND The sensor calibraiton file specified could not be found. Verify it's alongside the snapscan system file.
    \retval HSI_CONNECTION_FAILED Either the camera or the stage was impossible to connect. Verify both connections.
    */
    SNAPSCAN_API HSI_RETURN OpenDevice (HANDLE* o_p_device_handle, wchar_t const* i_system_file_path, bool i_dummy_api);

    /*!
    \brief Closes the connection to a snapscan device.

    \param[in,out] io_p_device_handle Pointer to a snapscan device handle. Will be set to NULL if closing the device succeeded.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle pointed to by io_p_device_handle is invalid.
    */
    SNAPSCAN_API HSI_RETURN CloseDevice (HANDLE* io_p_device_handle);

    /*!
    \brief Gets the device properties.

    \param[in] i_device_handle Handle to the device.
    \param[out] o_p_system_properties The system properties.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_system_properties is invalid.
    */
    SNAPSCAN_API HSI_RETURN GetSystemProperties (HANDLE i_device_handle, SystemProperties* o_p_system_properties);

    /*!
    \brief Gets the configuration parameters currently set.

    \pre This method can be called at any time.

    \param[in] i_device_handle Handle to the device.
    \param[in] o_p_configuration_parameters Pointer to a ConfigurationParameters structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_configuration_parameters is invalid.
    */
    SNAPSCAN_API HSI_RETURN GetConfigurationParameters (HANDLE i_device_handle, ConfigurationParameters* o_p_configuration_parameters);

    /*!
    \brief Sets the snapscan configuration parameters.

    \pre The snapscan must be in the stopped state.

    \param[in] i_device_handle Handle to the device.
    \param[in] i_configuration_parameters The configuration parameters. The parameters must be compatible with with the system (\sa GetSystemProperties).

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The snapscan is not in the correct state to handle this function call.
    \retval HSI_ARGUMENT_INVALID The given configuration parameters are not compatible with the snapscan system.
    */
    SNAPSCAN_API HSI_RETURN SetConfigurationParameters (HANDLE i_device_handle, ConfigurationParameters i_configuration_parameters);

    /*!
    \brief Sets a band-selection or spectral region of interest on the device.

    Sets a band-selection or spectral region of interest on the device. All future scans will only contains bands in this spectral range.
    To reset, provide one region of interest covering the entire spectral range of the device.

    \pre The snapscan must be in the stopped state.

    \param[in] i_device_handle Handle to the device.
    \param[in] i_spectral_regions Array of non-overlapping and sorted spectral regions of interest. (\sa SpectralRegionOfInterest).
    \param[in] i_nr_spectral_regions The number of spectral regions in array i_spectral_regions.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The snapscan is not in the correct state to handle this function call.
    \retval HSI_ARGUMENT_INVALID The given configuration parameters are not compatible with the snapscan system.
    */
    SNAPSCAN_API HSI_RETURN SetBandSelection (HANDLE i_device_handle, SpectralRegionOfInterest* i_spectral_regions, int i_nr_spectral_regions);

    /*!
    \brief Gets the data format of the cubes generated by the snapscan.

    \pre The snapscan must be in the ready or started state.

    \param[in] i_device_handle Handle to the device.
    \param[in] o_p_data_format Pointer to a CubeDataFormat structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_data_format is invalid.
    */
    SNAPSCAN_API HSI_RETURN GetOutputCubeDataFormat (HANDLE i_device_handle, CubeDataFormat* o_p_data_format);

    /*!
    \brief Gets the data format of the frames generated by the snapscan.

    \pre The snapscan must be in the ready or started state.

    \param[in] i_device_handle Handle to the device.
    \param[in] o_p_data_format Pointer to a FrameDataFormat structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_data_format is invalid.
    */
    SNAPSCAN_API HSI_RETURN GetOutputFrameDataFormat (HANDLE i_device_handle, FrameDataFormat* o_p_data_format);

    /*!
    \brief Initializes the snapscan system.

    \pre The snapscan must be in the stopped state.
    \post If the call succeeds with HSI_OK, the snapscan is in the ready state. Else the state remains unchanged.

    During this function call, the configuration parameters are used to allocate all internal buffers.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The snapscan is not in the correct state to handle this function call.
    \retval HSI_ALLOCATION_ERROR The allocation of internal buffers failed. Verify the system has sufficient resources.
    */
    SNAPSCAN_API HSI_RETURN Initialize (HANDLE i_device_handle);

    /*!
    \brief Stops the snapscan system.

    \pre It is recommended the snapscan is in the ready state.
    \post If the call succeeds with HSI_OK, the snapscan is in the stopped state. Else the state remains unchanged.

    During this function call, all internal buffers are deallocated.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    */
    SNAPSCAN_API HSI_RETURN Stop (HANDLE i_device_handle);

    /*!
    \brief Gets the correction matrix to spectrally correct the cubes generated by the snapscan.

    \pre The snapscan must be in the ready state.

    \param[in] i_device_handle Handle to the device.
    \param[out] o_p_correction_matrix Pointer to a CorrectionMatrix structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_correction_matrix is invalid.
    \retval HSI_CALL_ILLEGAL The snapscan is not in the correct state to handle this function call.
    */
    SNAPSCAN_API HSI_RETURN GetCorrectionMatrix (HANDLE i_device_handle, CorrectionMatrix* o_p_correction_matrix);

    /*!
    \brief Load the first correction correction matrix available to spectrally correct the cubes generated by the snapscan.

    The file must contain one correction matrix. If many, other are ignored.

    \param[out] o_p_correction_matrix Pointer to a CorrectionMatrix structure.
    \param[in]  i_in_file_path Path of the calibration file containing the matrix.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_correction_matrix is invalid.
    \retval HSI_FILE_NOT_FOUND The calibration file secified is not found.
    \retval HSI_FILE_IO_ERROR The calibration file specified cannot be read.
    */
    SNAPSCAN_API HSI_RETURN LoadCorrectionMatrix (CorrectionMatrix* o_p_correction_matrix, wchar_t const* i_in_file_path);

    /*!
    \brief Deallocates the memory of the correction matrix.

    \param[in] i_p_correction_matrix Pointer to the CorrectionMatrix structure to deallocate.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_correction_matrix is invalid.
    */
    SNAPSCAN_API HSI_RETURN DeallocateCorrectionMatrix (CorrectionMatrix* i_p_correction_matrix);

    /*!
    \brief Gets the runtime parameters currently set.

    \pre This method can be called at any time.

    \param[in] i_device_handle Handle to the device.
    \param[in] o_p_runtime_parameters Pointer to a RuntimeParameters structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_runtime_parameters is invalid.
    */
    SNAPSCAN_API HSI_RETURN GetRuntimeParameters (HANDLE i_device_handle, RuntimeParameters* o_p_runtime_parameters);

    /*!
    \brief Sets the snapscan runtime parameters.

    \pre The snapscan must be in the stopped or ready state (\sa Pauze).

    \param[in] i_device_handle Handle to the device.
    \param[in] i_runtime_parameters The runtime parameters. The parameters must be consistent with the allowed ranges.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The snapscan is not in the correct state to handle this function call.
    \retval HSI_ARGUMENT_INVALID The given runtime parameters are not consistent with the allowed ranges.
    */
    SNAPSCAN_API HSI_RETURN SetRuntimeParameters (HANDLE i_device_handle, RuntimeParameters i_runtime_parameters);

    /*!
    \brief Starts the snapscan system.

    \pre The snapscan must be in the ready state.
    \post If the call succeeds with HSI_OK, the snapscan is in the started state. Else the state remains unchanged.

    During this function call, all runtime parameters are applied after which the system is ready to acquire data.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The snapscan is not in the correct state to handle this function call.
    */
    SNAPSCAN_API HSI_RETURN Start (HANDLE i_device_handle);

    /*!
    \brief Pauses the snapscan system.

    \pre It is recommended the snapscan is in the started state.
    \post If the call succeeds with HSI_OK, the snapscan is in the ready state. Else the state remains unchanged.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    */
    SNAPSCAN_API HSI_RETURN Pause (HANDLE i_device_handle);

    /*!
    \brief Starts the acquisition of a dark reference frame.

    The snapscan system starts capturing a dark reference frame. The data is placed in o_p_frame.

    \pre The snapscan must be in the started state (\sa Start). The frame must be either freshly created or unallocated (\sa DeallocateFrame).

    \param[in] i_device_handle Handle to the device.
    \param[in] o_p_frame Pointer to a FrameFloat structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The frame is pointing to null or the structure is already in use.
    \retval HSI_CALL_ILLEGAL The snapscan is not in the correct state to handle this function call.
    \retval HSI_ACQUISITON_FAILED. Something occured during the data acquisition. Verify the available bandwidth to the camera and the CPU usage and the physical RAM availability.
    */
    SNAPSCAN_API HSI_RETURN AcquireDarkReferenceFrame (HANDLE i_device_handle, FrameFloat* o_p_frame);

    /*!
    \brief Starts the acquisition of a cube.

    The snapscan system acquires a dark reference and starts making a scan as specified by the set configuration and runtime parameters.
    The dark reference is removed during creation of the scan. The scan data is placed in o_p_cube.

    \pre The snapscan must be in the started state (\sa Start). The data behind the specified cube must be allocated (\sa AllocateCube).

    \param[in] i_device_handle Handle to the device.
    \param[in] i_p_dark_ref_frame Pointer to the dark reference frame.
    \param[in,out] o_p_cube Pointer to a CubeFloat structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The snapscan is not in the correct state to handle this function call.
    \retval HSI_DATA_NOT_ALLOCATED The referenced data structure o_p_cube is not allocated (\sa AllocateCube).
    \retval HSI_ACQUISITON_FAILED. Something occured during the data acquisition. Verify the available bandwidth to the camera, the CPU usage, the physical RAM availability.
    */
    SNAPSCAN_API HSI_RETURN AcquireCube (HANDLE i_device_handle, FrameFloat* i_p_dark_ref_frame, CubeFloat* o_p_cube);

    /*!
    \brief Moves the stage to the position.

    The snapscan system moves its internal translation stage to a distance expressed in pixel from the center.

    \pre The snapscan must be in the started state (\sa Start).

    \param[in] i_device_handle Handle to the device.
    \param[in] i_position_px The distance expressed in pixel from the center. Must be within ]-stage_position_px_max, stage_position_px_max[ (\sa SystemProperties)

    */
    SNAPSCAN_API HSI_RETURN MoveStage (HANDLE i_device_handle, int i_position_px);

    /*!
    \brief Starts the acquisition of a frame.

    The snapscan system starts making a raw scan of the active parts of the sensor. The scan data is placed in o_p_frame.

    \pre The snapscan must be in the started state (\sa Start). The data behind the specified cube must be allocated (\sa AllocateFrame).

    \param[in] i_device_handle Handle to the device.
    \param[in] i_timeout_ms Timeout in milliseconds. If less or equal to 0, the call will never time-out.
    \param[in] o_p_frame Pointer to a FrameFloat structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The snapscan is not in the correct state to handle this function call.
    \retval HSI_DATA_NOT_ALLOCATED The referenced data structure o_p_frame is not allocated (\sa AllocateFrame).
    \retval HSI_ACQUISITION_TIMEOUT No frame could be acquired within the specified timeout period.
    \retval HSI_ACQUISITON_FAILED. Something occured during the data acquisition. Verify the available bandwidth to the camera and the CPU usage and the physical RAM availability.
    */
    SNAPSCAN_API HSI_RETURN AcquireFrame (HANDLE i_device_handle, int i_timeout_ms, FrameFloat* o_p_frame);

    /*!
    \brief Aborts an ongoing acquisition.

    \pre It is recommended the snapscan is acquiring a cube.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    */
    SNAPSCAN_API HSI_RETURN AbortAcquisition (HANDLE i_device_handle);

    /*!
    \brief Apply white reflectance to the object data by dividing by a reference.

    \pre Both object and reference must be valid and may not be reflectance data. Both object and reference must have the same value of the vignetting corrected flag.
    \post The result object will be valid and vignetting corrected.

    The object data in i_p_object is normalized by the reference data in i_p_reference. The normalization assumes a linear response
    with integration time and gain to account for differences in these parameters.

    \param[out] o_p_result Pointer to a CubeFloat structure in which to put the normalized data. May point to i_p_object.
    \param[in] i_object Object cube data to normalize.
    \param[in] i_p_reference Reference cube data by which to normalize i_p_object.
    \param[in] i_white_value The value of the white reference (e.g. 0.95). Must be within [0,1].

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_normalized_object is invalid.
    \retval HSI_DATA_NOT_ALLOCATED One of the provided data structures is not allocated (\sa AllocateCube).
    \retval HSI_DATA_NOT_VALID Either of the input data i_p_object or i_p_reference has valid flag false and cannot be used for further processing.
    \retval HSI_DATA_NOT_COMPATIBLE The data in i_p_object is not compatible with the data in i_p_reference. Verify data format and data flags.
    */

    SNAPSCAN_API HSI_RETURN ApplyWhiteReference (CubeFloat* o_p_result, CubeFloat i_object, CubeFloat i_p_reference, double i_white_value);
    /*!
    \brief Applies a spectral correction matrix to the data.

    \pre The object data must be valid and must be reflectance data. 
    \post The normalized object will be valid and vignetting corrected.

    \note The dimensions of o_p_corrected_object, i_object and i_correction_matrix must match: The i_object structure must have as many bands as there
    are columns in i_correction_matrix (\sa AllocateCube"CubeFloat*,CubeDataFormat"). The data cube behind o_p_corrected_object must have as many bands
    as there are rows in i_correction_matrix (\sa AllocateCube"CubeFloat*,CorrectionMatrix").
    
    \param[out] o_p_corrected_object Pointer to a CubeFloat structure allocated with AllocateCube"CubeFloat*,CorrectionMatrix" in which to put the corrected data. May not point to i_object.
    \param[in] i_object Object cube data to apply spectral correction to (\sa AllocateCube"CubeFloat*,CubeDataFormat").
    \param[in] i_correction_matrix Correction matrix to apply to i_object (\sa GetCorrectionMatrix).

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_corrected_object is invalid.
    \retval HSI_DATA_NOT_ALLOCATED One of the provided data structures is not allocated (\sa AllocateCube and \sa GetCorrectionMatrix).
    \retval HSI_DATA_NOT_VALID The input data i_p_object has valid flag false and cannot be used for further processing.
    \retval HSI_DATA_NOT_COMPATIBLE The data in i_p_object or behind o_p_corrected_object is not compatible with i_correction_matrix. Verify data format and data flags.
    */
    SNAPSCAN_API HSI_RETURN ApplySpectralCorrection (CubeFloat* o_p_corrected_object, CubeFloat i_object, CorrectionMatrix i_correction_matrix);

    /*!
    \brief Allocates a data cube that can host corrected data compatible with a certain correction matrix and input data format.

    \param[out] o_p_cube Pointer to the cube structure in which to allocate the data.
    \param[in] i_correction_matrix Correction matrix that will be used to fill in the allocated data.
    \param[in] i_data_format Data format of the input data for spectral correction.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_cube is invalid or the cube has already been allocated (\sa DeallocateCube).
    \retval HSI_DATA_NOT_COMPATIBLE The input data format is not compatible with the correction matrix.
    \retval HSI_ALLOCATION_ERROR The allocation failed. Verify the system has sufficient resources.
    */
    SNAPSCAN_API HSI_RETURN AllocateCubeCorrected (CubeFloat* o_p_cube, CorrectionMatrix i_correction_matrix, CubeDataFormat i_data_format);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SNAPSCAN_API_H