/******************************************************************************
**+-------------------------------------------------------------------------+**
**|                            ****                                         |**
**|                            ****                                         |**
**|                            ******o***                                   |**
**|                      ********_///_****                                  |**
**|                      ***** /_//_/ ****                                  |**
**|                       ** ** (__/ ****                                   |**
**|                           *********                                     |**
**|                            ****                                         |**
**|                            ***                                          |**
**|                                                                         |**
**|         Copyright (c) 1998-2007 Texas Instruments Incorporated          |**
**|                        ALL RIGHTS RESERVED                              |**
**|                                                                         |**
**| Permission is hereby granted to licensees of Texas Instruments          |**
**| Incorporated (TI) products to use this computer program for the sole    |**
**| purpose of implementing a licensee product based on TI products.        |**
**| No other rights to reproduce, use, or disseminate this computer         |**
**| program, whether in part or in whole, are granted.                      |**
**|                                                                         |**
**| TI makes no representation or warranties with respect to the            |**
**| performance of this computer program, and specifically disclaims        |**
**| any responsibility for any damages, special or consequential,           |**
**| connected with the use of this program.                                 |**
**|                                                                         |**
**+-------------------------------------------------------------------------+**
******************************************************************************/

/** \file   fvid.h
 *
 *  \brief  This file is used by the Application to call the GIO functions
 *
 *
 *  (C) Copyright 2006, Texas Instruments, Inc
 *
 *  \author     PSP Team
 *
 *  \version    0.1     Created
 */
#ifndef _FVID_H
#define _FVID_H

/*******Header File Inclusion *******/
#include <std.h>
#include <gio.h>
#include <iom.h>

#include <ti/sdo/pspdrivers/common/psp_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * =========== command codes for IOM_Packet =============
 */

/** \brief IOM user defined command base address */
#define FVID_BASE          (IOM_USER)

/**
 *  \brief command for FVID_alloc to request the video device driver to give
 *  ownership of a data buffer
 */
#define FVID_ALLOC         (FVID_BASE + 0)

/**
 *  \brief command for FVID_free to submit a video buffer back to video device
 *  driver
 */
#define FVID_FREE          (FVID_BASE + 1)

/**
 *  \brief command for FVID_exchange to exchange buffers between Driver and
 *  Application
 */
#define FVID_EXCHANGE      (FVID_BASE + 2)

/**
 *  \brief command for FVID_queue to submit a video buffer back to video device
 *  driver
 */
#define FVID_QUEUE         (FVID_BASE + 3)

/**
 *  \brief command for FVID_dequeue to request the video device driver to give
 *  ownership of a data buffer
 */
#define FVID_DEQUEUE       (FVID_BASE + 4)

/**
 *  \brief command for FVID_allocBuffer to request the video device driver to
 *  allocate one data buffer
 */
#define FVID_ALLOC_BUFFER  (FVID_BASE + 5)

/**
 *  \brief command for FVID_freeBuffer to request the video device driver to
 *  free memory of given data buffer
 */
#define FVID_FREE_BUFFER   (FVID_BASE + 6)

/** \brief Driver channel handle */
typedef GIO_Handle FVID_Handle;

/**
 *  \brief Interlaced frame.
 *  Interlaced frame has two fields in one frame
 */
typedef struct FVID_IFrame {
    Char* y1;
    /**< Character pointer for field 1 Y data */
    Char* cb1;
    /**< Character pointer for field 1 CB data */
    Char* cr1;
    /**< Character pointer for field 1 CR data */
    Char* y2;
    /**< Character pointer for field 2 Y data */
    Char* cb2;
    /**< Character pointer for field 2 CB data */
    Char* cr2;
    /**< Character pointer for field 2 CR data */
} FVID_IFrame;

/**
  * \brief Definition of Progressive Frame
  *
  */
typedef struct FVID_PFrame {
    Char* y;
    /**< Character pointer for frame Y data */
    Char* cb;
    /**< Character pointer for frame CB data */
    Char* cr;
    /**< Character pointer for frame CR data */
} FVID_PFrame;

/**
  * \brief Definition of Raw frame (Interlaced). Could be RGB,
  *        monochrome or any type of data interleaved Y/C frame etc.
  *
  */
typedef struct FVID_RawIFrame {
    Char* buf1;
    /**< Character pointer for field 1 */
    Char* buf2;
    /**< Character pointer for field 2 */
} FVID_RawIFrame;

/**
  * \brief Definition of Raw frame (Progressive). Could be RGB,
  *        monochrome or any type of data interleaved Y/C frame etc.
  *
  */
typedef struct FVID_RawPFrame {
    Char* buf;
    /**< Character pointer for frame */
} FVID_RawPFrame;

/**
 *  \brief Enum for color format
 */
typedef enum _FVID_colorFormat {
    FVID_YCbCr422_INTERLEAVED = 0,
    FVID_YCbCr422_PLANAR,
    FVID_YCrCb422_INTERLEAVED,
    FVID_RGB_888_INTERLEAVED,
    FVID_RGB565_INTERLEAVED,
    FVID_DVD_MODE,
    FVID_CLUT_INDEXED,
    FVID_ATTRIBUTE,
    FVID_BAYER_PATTERN,
    FVID_RAW_FORMAT,
    FVID_COLORFORMAT_INVALID
} FVID_colorFormat; /**< Typedef for enum _FVID_colorFormat */

/**
 *  \brief Enum for video interface
 */
typedef enum _FVID_videoInterface {
    FVID_VI_BT656_8BIT,
    /**< 8-bit BT.656 interface with embedded sync */
    FVID_VI_BT656_10BIT,
    /**< 10-bit BT.656 interface with embedded sync */
    FVID_VI_YC_8BIT_CS,
    /**< 8-bit YC interface with external control sync */
    FVID_VI_YC_10BIT_CS,
    /**< 10-bit YC interface with external control sync */
    FVID_VI_YC_16BIT_ES,
    /**< 16-bit YC interface with embedded sync */
    FVID_VI_YC_16BIT_CS,
    /**< 16-bit YC interface with external control sync */
    FVID_VI_RAW_8BIT_CS,
    /**< 8-bit RAW interface with external control sync */
    FVID_VI_RAW_10BIT_CS,
    /**< 10-bit RAW interface with external control sync */
    FVID_VI_RAW_16BIT_CS,
    /**< 16-bit RAW interface with external control sync */
    FVID_VIDEOINTERFACE_INVALID
} FVID_videoInterface;  /**< Typedef for enum _FVID_videoInterface */

/**
  * \brief Enum for the Field Frame Modes
  *
  */
typedef enum _FVID_FieldFrame
{
    FVID_FIELD_MODE = 0,
    /**< Progressive Mode   */
    FVID_FRAME_MODE
    /**< Interlaced Mode    */
} FVID_FieldFrame;  /**< Typedef for enum _FVID_FieldFrame */

/** \brief Enum for the Bits Per Pixel for different Module */
typedef enum _FVID_bitsPerPixel {
    FVID_BPP_BITS1  = 1,
    FVID_BPP_BITS2  = 2,
    FVID_BPP_BITS4  = 4,
    FVID_BPP_BITS8  = 8,
    FVID_BPP_BITS16 = 16,
    FVID_BPP_BITS24 = 24
} FVID_bitsPerPixel;    /**< Typedef for enum _FVID_bitsPerPixel */

/**
  * \brief FVID frame buffer descriptor
  */
typedef struct _FVID_Frame {
    QUE_Elem            queElement;
    /**< for queuing */

    union {
        FVID_IFrame     iFrm;
        /**< y/c frame buffer for interlaced mode         */
        FVID_PFrame     pFrm;
        /**< y/c frame buffer for progressive mode        */
        FVID_RawIFrame  riFrm;
        /**< raw frame buffer for interlaced mode         */
        FVID_RawPFrame  rpFrm;
        /**< raw frame buffer for progressive mode        */
        Ptr             frameBufferPtr;
    } frame;    /**< \brief union for frame type as used by driver */

    Uint32              timeStamp;
    /**< Time Stamp for captured or displayed frame */

    Uint32              pitch;
    /**< Pitch parameters for given plane */

    Uint32              lines;
    /**< Number of lines per frame */

    FVID_bitsPerPixel   bpp;
    /**< Number of bits per pixel */

    FVID_colorFormat    frameFormat;
    /**< Frame Color Format */

    Ptr                 userParams;
    /**< In/Out Additional User Parameters per frame */

    Ptr                 misc;
    /**< For future use */
} FVID_Frame;   /**< Typedef for structure _FVID_Frame */


/**
 *  \brief ======== FVID_control ========
 *  An application calls FVID_control to send device-specific control
 *  commands to the mini-driver.
 */
#define FVID_control(gioChan, cmd, args) \
        GIO_control(gioChan, cmd, args)


/**
 *  \brief ======== FVID_create ========
 *  FVID_create() allocates and initializes an GIO_Obj structure. FVID_create()
 *  returns a non-NULL GIO_Handle object on success and NULL for failure.
 *  The 'name' parameter is used to find a matching name in the device
 *  table.  Associated IOM_Fxns table and params structure are then used
 *  to create a channel for that device.   The 'attrs->nPackets' parameter
 *  specifies the number of asynchronous I/O calls that can be outstanding.
 *  The status field is the address where a device status is returned, ignored
 *  if status is specified as NULL.
 */
#define FVID_create(name, mode, status, optArgs, attrs) \
            GIO_create(name, mode, status, optArgs, attrs)


/**
 *  \brief ======== FVID_delete ========
 *  Application calls FVID_delete to close the logical channel
 *  associated with gioChan.
 */
#define FVID_delete(gioChan) \
            GIO_delete(gioChan)

/**
 *  \brief ======== FVID_exchange ========
 *  Exchange one driver-managed buffer for another driver-managed buffer.
 *  This operation is similar to an FVID_free()/alloc() pair but has less
 *  overhead since it involves only one call into the driver.
 */
#define FVID_exchange(gioChan, bufp) \
            GIO_submit(gioChan, FVID_EXCHANGE, bufp, NULL, NULL)

/**
 *  \brief ======== FVID_alloc ========
 *  FVID_alloc() is used for device drivers that manage their own buffers.
 *  FVID_alloc() returns a pointer to a device driver allocated buffer and
 *  also sets the size.
 */
#define FVID_alloc(gioChan, bufp) \
            GIO_submit(gioChan, FVID_ALLOC, bufp, NULL, NULL)

/**
 *  \brief ======== FVID_free ========
 *  An application calls FVID_free to relinquish a video buffer back to
 *  the video device driver .
 */
#define FVID_free(gioChan, bufp) \
            GIO_submit(gioChan, FVID_FREE, bufp, NULL, NULL)

/**
 *  \brief ======== FVID_allocBuffer ========
 *  \brief  An application calls FVID_allocBuffer to request the video device
 *  driver to allocate one data buffer.
 */
#define FVID_allocBuffer(gioChan, bufp) \
            GIO_control(gioChan, FVID_ALLOC_BUFFER, bufp)

/**
 *  \brief ======== FVID_freeBuffer ========
 *  An application calls FVID_freeBuffer to request the video device
 *  driver to free memory of given data buffer.
 *  FVID_freeBuffer() frees the buffer allocated via FVID_allocBuffer().
 */
#define FVID_freeBuffer(gioChan, bufp) \
            GIO_control(gioChan, FVID_FREE_BUFFER, bufp)

/**
 *  \brief ======== FVID_queue ========
 *  An application calls FVID_queue to submit a video buffer to the video
 *  device driver.
 */
#define FVID_queue(giochan,bufp) \
            GIO_submit(giochan,FVID_QUEUE,bufp,NULL,NULL)

/**
 *  \brief ======== FVID_dequeue ========
 *  An application calls FVID_dequeue to request the video device
 *  driver to give ownership of a data buffer.
 */
#define FVID_dequeue(giochan,bufp) \
            GIO_submit(giochan,FVID_DEQUEUE,bufp,NULL,NULL)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* _FVID_H */

/******************************************************************************/
/*  End of file                                                               */
/******************************************************************************/
