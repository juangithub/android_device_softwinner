/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HW_EMULATOR_CAMERA_EMULATED_CAMERA_DEVICE_H
#define HW_EMULATOR_CAMERA_EMULATED_CAMERA_DEVICE_H

/*
 * Contains declaration of an abstract class V4L2Camera that defines
 * functionality expected from an emulated physical camera device:
 *  - Obtaining and setting camera device parameters
 *  - Capturing frames
 *  - Streaming video
 *  - etc.
 */

#include <utils/threads.h>
#include "CameraCommon.h"

namespace android {

#define NB_BUFFER 5

class CameraHardware;

/* Encapsulates an abstract class V4L2Camera that defines
 * functionality expected from an emulated physical camera device:
 *  - Obtaining and setting camera device parameters
 *  - Capturing frames
 *  - Streaming video
 *  - etc.
 */
class V4L2Camera {
public:
    /* Constructs V4L2Camera instance.
     * Param:
     *  camera_hal - V4L2Camera that implements the camera HAL API, and
     *      manages (contains) this object.
     */
    explicit V4L2Camera(CameraHardware* camera_hal);

    /* Destructs V4L2Camera instance. */
    virtual ~V4L2Camera();

    /***************************************************************************
     * V4L2Camera device abstract interface
     **************************************************************************/

public:
    /* Connects to the camera device.
     * This method must be called on an initialized instance of this class.
     * Return:
     *  NO_ERROR on success, or an appropriate error status.
     */
    virtual status_t connectDevice() = 0;

    /* Disconnects from the camera device.
     * Return:
     *  NO_ERROR on success, or an appropriate error status. If this method is
     *  called for already disconnected, or uninitialized instance of this class,
     *  a successful status must be returned from this method. If this method is
     *  called for an instance that is in the "started" state, this method must
     *  return a failure.
     */
    virtual status_t disconnectDevice() = 0;

    /* Starts the camera device.
     * This method tells the camera device to start capturing frames of the given
     * dimensions for the given pixel format. Note that this method doesn't start
     * the delivery of the captured frames to the V4L2Camera. Call
     * startDeliveringFrames method to start delivering frames. This method must
     * be called on a connected instance of this class. If it is called on a
     * disconnected instance, this method must return a failure.
     * Param:
     *  width, height - Frame dimensions to use when capturing video frames.
     *  pix_fmt - Pixel format to use when capturing video frames.
     * Return:
     *  NO_ERROR on success, or an appropriate error status.
     */
    virtual status_t startDevice(int width, int height, uint32_t pix_fmt, bool video_hint) = 0;

    /* Stops the camera device.
     * This method tells the camera device to stop capturing frames. Note that
     * this method doesn't stop delivering frames to the V4L2Camera. Always
     * call stopDeliveringFrames prior to calling this method.
     * Return:
     *  NO_ERROR on success, or an appropriate error status. If this method is
     *  called for an object that is not capturing frames, or is disconnected,
     *  or is uninitialized, a successful status must be returned from this
     *  method.
     */
    virtual status_t stopDevice() = 0;

    /***************************************************************************
     * V4L2Camera device public API
     **************************************************************************/

public:
    /* Initializes V4L2Camera instance.
     * Derived classes should override this method in order to cache static
     * properties of the physical device (list of supported pixel formats, frame
     * sizes, etc.) If this method is called on an already initialized instance,
     * it must return a successful status.
     * Return:
     *  NO_ERROR on success, or an appropriate error status.
     */
    virtual status_t Initialize();

    /* Starts delivering frames captured from the camera device.
     * This method will start the worker thread that would be pulling frames from
     * the camera device, and will deliver the pulled frames back to the emulated
     * camera via onNextFrameAvailable callback. This method must be called on a
     * connected instance of this class with a started camera device. If it is
     * called on a disconnected instance, or camera device has not been started,
     * this method must return a failure.
     * Param:
     *  one_burst - Controls how many frames should be delivered. If this
     *      parameter is 'true', only one captured frame will be delivered to the
     *      V4L2Camera. If this parameter is 'false', frames will keep
     *      coming until stopDeliveringFrames method is called. Typically, this
     *      parameter is set to 'true' only in order to obtain a single frame
     *      that will be used as a "picture" in takePicture method of the
     *      V4L2Camera.
     * Return:
     *  NO_ERROR on success, or an appropriate error status.
     */
    virtual status_t startDeliveringFrames(bool one_burst);

    /* Stops delivering frames captured from the camera device.
     * This method will stop the worker thread started by startDeliveringFrames.
     * Return:
     *  NO_ERROR on success, or an appropriate error status.
     */
    virtual status_t stopDeliveringFrames();

    /* Gets width of the frame obtained from the physical device.
     * Return:
     *  Width of the frame obtained from the physical device. Note that value
     *  returned from this method is valid only in case if camera device has been
     *  started.
     */
    inline int getFrameWidth() const
    {
        LOGE_IF(!isStarted(), "%s: Device is not started", __FUNCTION__);
        return mFrameWidth;
    }

    /* Gets height of the frame obtained from the physical device.
     * Return:
     *  Height of the frame obtained from the physical device. Note that value
     *  returned from this method is valid only in case if camera device has been
     *  started.
     */
    inline int getFrameHeight() const
    {
        LOGE_IF(!isStarted(), "%s: Device is not started", __FUNCTION__);
        return mFrameHeight;
    }

    /* Gets byte size of the current frame buffer.
     * Return:
     *  Byte size of the frame buffer. Note that value returned from this method
     *  is valid only in case if camera device has been started.
     */
    inline size_t getFrameBufferSize() const
    {
        LOGE_IF(!isStarted(), "%s: Device is not started", __FUNCTION__);
        return mFrameBufferSize;
    }

    /* Gets number of pixels in the current frame buffer.
     * Return:
     *  Number of pixels in the frame buffer. Note that value returned from this
     *  method is valid only in case if camera device has been started.
     */
    inline int getPixelNum() const
    {
        LOGE_IF(!isStarted(), "%s: Device is not started", __FUNCTION__);
        return mTotalPixels;
    }

    /* Gets pixel format of the frame that camera device streams to this class.
     * Throughout camera framework, there are three different forms of pixel
     * format representation:
     *  - Original format, as reported by the actual camera device. Values for
     *    this format are declared in bionic/libc/kernel/common/linux/videodev2.h
     *  - String representation as defined in CameraParameters::PIXEL_FORMAT_XXX
     *    strings in frameworks/base/include/camera/CameraParameters.h
     *  - HAL_PIXEL_FORMAT_XXX format, as defined in system/core/include/system/graphics.h
     * Since V4L2Camera device gets its data from the actual device, it gets
     * pixel format in the original form. And that's the pixel format
     * representation that will be returned from this method. HAL components will
     * need to translate value returned from this method to the appropriate form.
     * This method must be called only on started instance of this class, since
     * it's applicable only when camera device is ready to stream frames.
     * Param:
     *  pix_fmt - Upon success contains the original pixel format.
     * Return:
     *  Current framebuffer's pixel format. Note that value returned from this
     *  method is valid only in case if camera device has been started.
     */
    inline uint32_t getOriginalPixelFormat() const
    {
        LOGE_IF(!isStarted(), "%s: Device is not started", __FUNCTION__);
        return mPixelFormat;
    }

    /*
     * State checkers.
     */

    inline bool isInitialized() const {
        /* Instance is initialized when the worker thread has been successfuly
         * created (but not necessarily started). */
        return mWorkerThread.get() != NULL && mState != ECDS_CONSTRUCTED;
    }
    inline bool isConnected() const {
        /* Instance is connected when its status is either"connected", or
         * "started". */
        return mState == ECDS_CONNECTED || mState == ECDS_STARTED;
    }
    inline bool isStarted() const {
        return mState == ECDS_STARTED;
    }

    /****************************************************************************
     * V4L2Camera device private API
     ***************************************************************************/
protected:
    /* Performs common validation and calculation of startDevice parameters.
     * Param:
     *  width, height, pix_fmt - Parameters passed to the startDevice method.
     * Return:
     *  NO_ERROR on success, or an appropriate error status.
     */
    virtual status_t commonStartDevice(int width, int height, uint32_t pix_fmt);

    /* Performs common cleanup on stopDevice.
     * This method will undo what commonStartDevice had done.
     */
    virtual void commonStopDevice();

    /****************************************************************************
     * Worker thread management.
     * Typicaly when V4L2Camera device starts capturing frames from the
     * actual device, it does that in a worker thread created in StartCapturing,
     * and terminated in StopCapturing. Since this is such a typical scenario,
     * it makes sence to encapsulate worker thread management in the base class
     * for all V4L2Camera devices.
     ***************************************************************************/

protected:
    /* Starts the worker thread.
     * Typically, worker thread is started from startDeliveringFrames method of
     * this class.
     * Param:
     *  one_burst - Controls how many times thread loop should run. If this
     *      parameter is 'true', thread routine will run only once If this
     *      parameter is 'false', thread routine will run until stopWorkerThread
     *      method is called. See startDeliveringFrames for more info.
     * Return:
     *  NO_ERROR on success, or an appropriate error status.
     */
    virtual status_t startWorkerThread(bool one_burst);

    /* Stops the worker thread.
     * Note that this method will always wait for the worker thread to terminate.
     * Typically, worker thread is started from stopDeliveringFrames method of
     * this class.
     * Return:
     *  NO_ERROR on success, or an appropriate error status.
     */
    virtual status_t stopWorkerThread();

    /* Implementation of the worker thread routine.
     * In the default implementation of the worker thread routine we simply
     * return 'false' forcing the thread loop to exit, and the thread to
     * terminate. Derived class should override that method to provide there the
     * actual frame delivery.
     * Return:
     *  true To continue thread loop (this method will be called again), or false
     *  to exit the thread loop and to terminate the thread.
     */
    virtual bool inWorkerThread();

    /* Encapsulates a worker thread used by the V4L2Camera device.
     */
    friend class WorkerThread;
    class WorkerThread : public Thread {

        /****************************************************************************
         * Public API
         ***************************************************************************/

        public:
            inline explicit WorkerThread(V4L2Camera* camera_dev)
                : Thread(true),   // Callbacks may involve Java calls.
                  mCameraDevice(camera_dev),
                  mThreadControl(-1),
                  mControlFD(-1)
            {
            }

            inline ~WorkerThread()
            {
                LOGW_IF(mThreadControl >= 0 || mControlFD >= 0,
                        "%s: Control FDs are opened in the destructor",
                        __FUNCTION__);
                if (mThreadControl >= 0) {
                    close(mThreadControl);
                }
                if (mControlFD >= 0) {
                    close(mControlFD);
                }
            }

            /* Starts the thread
             * Param:
             *  one_burst - Controls how many times thread loop should run. If
             *      this parameter is 'true', thread routine will run only once
             *      If this parameter is 'false', thread routine will run until
             *      stopThread method is called. See startWorkerThread for more
             *      info.
             * Return:
             *  NO_ERROR on success, or an appropriate error status.
             */
            inline status_t startThread(bool one_burst)
            {
                mOneBurst = one_burst;
                return run(NULL, ANDROID_PRIORITY_URGENT_DISPLAY, 0);
            }

            /* Overriden base class method.
             * It is overriden in order to provide one-time initialization just
             * prior to starting the thread routine.
             */
            status_t readyToRun();

            /* Stops the thread. */
            status_t stopThread();

            /* Values returned from the Select method of this class. */
            enum SelectRes {
                /* A timeout has occurred. */
                TIMEOUT,
                /* Data are available for read on the provided FD. */
                READY,
                /* Thread exit request has been received. */
                EXIT_THREAD,
                /* An error has occurred. */
                ERROR
            };

            /* Select on an FD event, keeping in mind thread exit message.
             * Param:
             *  fd - File descriptor on which to wait for an event. This
             *      parameter may be negative. If it is negative this method will
             *      only wait on a control message to the thread.
             *  timeout - Timeout in microseconds. 0 indicates no timeout (wait
             *      forever).
             * Return:
             *  See SelectRes enum comments.
             */
            SelectRes Select(int fd, int timeout);

        /****************************************************************************
         * Private API
         ***************************************************************************/

        private:
            /* Implements abstract method of the base Thread class. */
            bool threadLoop()
            {
                /* Simply dispatch the call to the containing camera device. */
                if (mCameraDevice->inWorkerThread()) {
                    /* Respect "one burst" parameter (see startThread). */
                    return !mOneBurst;
                } else {
                    return false;
                }
            }

            /* Containing camera device object. */
            V4L2Camera*   mCameraDevice;

            /* FD that is used to send control messages into the thread. */
            int                     mThreadControl;

            /* FD that thread uses to receive control messages. */
            int                     mControlFD;

            /* Controls number of times the thread loop runs.
             * See startThread for more information. */
            bool                    mOneBurst;

            /* Enumerates control messages that can be sent into the thread. */
            enum ControlMessage {
                /* Stop the thread. */
                THREAD_STOP
            };
    };

    /* Worker thread accessor. */
    inline WorkerThread* getWorkerThread() const
    {
        return mWorkerThread.get();
    }

    /****************************************************************************
     * Data members
     ***************************************************************************/

protected:
    /* Locks this instance for parameters, state, etc. change. */
    Mutex                       mObjectLock;

    /* Worker thread that is used in frame capturing. */
    sp<WorkerThread>            mWorkerThread;

    /* Timestamp of the current frame. */
    nsecs_t                     mCurFrameTimestamp;

    /* V4L2Camera object containing this instance. */
    CameraHardware*             mCameraHAL;

    /*
     * Framebuffer properties.
     */

    /* Byte size of the framebuffer. */
    size_t                      mFrameBufferSize;

    /* Original pixel format (one of the V4L2_PIX_FMT_XXX values, as defined in
     * bionic/libc/kernel/common/linux/videodev2.h */
    uint32_t                    mPixelFormat;

    /* Frame width */
    int                         mFrameWidth;

    /* Frame height */
    int                         mFrameHeight;

    /* Total number of pixels */
    int                         mTotalPixels;

    /* Defines possible states of the V4L2Camera device object.
     */
    enum EmulatedCameraDeviceState {
        /* Object has been constructed. */
        ECDS_CONSTRUCTED,
        /* Object has been initialized. */
        ECDS_INITIALIZED,
        /* Object has been connected to the physical device. */
        ECDS_CONNECTED,
        /* Camera device has been started. */
        ECDS_STARTED,
    };

    /* Object state. */
    EmulatedCameraDeviceState   mState;

	// -------------------------------------------------------------------------
	// extended interfaces here <***** star *****>
	// -------------------------------------------------------------------------
	
protected:
	bool						mTakingPicture;
	bool						mTakingPictureRecord;
	int							mPictureWidth;
	int							mPictureHeight;
	
	pthread_mutex_t 			mTakePhotoMutex;
	pthread_cond_t				mTakePhotoCond;

	bool						mThreadRunning;	
	pthread_mutex_t 			mThreadRunningMutex;
	pthread_cond_t				mThreadRunningCond;
	
public:
	
	inline void setTakingPicture(bool taking)
	{
		pthread_mutex_lock(&mTakePhotoMutex);
		LOGV("setTakingPicture : %s", taking ? "true" : "false");
		mTakingPicture = taking;
		pthread_mutex_unlock(&mTakePhotoMutex);
	}

	inline void setTakingPictureRecord(bool taking)
	{
		pthread_mutex_lock(&mTakePhotoMutex);
		LOGV("setTakingPicture : %s", taking ? "true" : "false");
		mTakingPictureRecord = taking;
		pthread_mutex_unlock(&mTakePhotoMutex);
	}
	
	inline int getPictureSize(int * pic_w, int * pic_h)
    {
        *pic_w = mPictureWidth;
		*pic_h = mPictureHeight;
		return OK;
    }
	
	inline int setPictureSize(int pic_w, int pic_h)
    {
        mPictureWidth = pic_w;
		mPictureHeight = pic_h;
		return OK;
    }

	inline void setThreadRunning(bool running)
	{
		pthread_mutex_lock(&mThreadRunningMutex);
		if (!mThreadRunning)
		{
			LOGV("setThreadRunning true");
			mThreadRunning = running;
			pthread_cond_signal(&mThreadRunningCond);
		}
		pthread_mutex_unlock(&mThreadRunningMutex);
	}

	inline bool getThreadRunning()
	{
		return mThreadRunning;
	}
};

}; /* namespace android */

#endif  /* HW_EMULATOR_CAMERA_EMULATED_CAMERA_DEVICE_H */