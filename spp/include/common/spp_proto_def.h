#ifndef SPP_PROTO_DEF_H
#define SPP_PROTO_DEF_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/**
 * Message IDs
 */

#define SPP_MSG_HDR_SIZE(address_length)    (2 + address_length * 2)
#define SPP_MSG_CONNECT_REQUEST_BC_ID       ((uint8_t)0xA0)
#define SPP_MSG_CONNECT_REQUEST_BC_SIZE     (2)
#define SPP_MSG_CONNECT_RESPONSE_ID         ((uint8_t)0xA1)
#define SPP_MSG_CONNECT_RESPONSE_SIZE       (1) 
#define SPP_MSG_HOST_ALIVE_BC_ID            ((uint8_t)0xA2)
#define SPP_MSG_HOST_ALIVE_BC_SIZE          (0)
#define SPP_MSG_PROP_LIST_REQUEST_ID        ((uint8_t)0xA3)
#define SPP_MSH_PROP_LIST_REQUEST_SIZE      (0)
#define SPP_MSG_PROP_LIST_RESPONSE_ID       ((uint8_t)0xA4)
#define SPP_MSG_PROP_LIST_RESPONSE_SIZE     (4)
#define SPP_MSG_PROP_DEF_ID                 ((uint8_t)0xA5)
#define SPP_MSG_PROP_DEF_SIZE               (6)
#define SPP_MSG_GET_REQUEST_ID              ((uint8_t)0xB0)
#define SPP_MSG_GET_REQUEST_SIZE            (3)
#define SPP_MSG_GET_RESPONSE_ID             ((uint8_t)0xB1)
#define SPP_MSG_GET_RESPONSE_BASE_SIZE      (5)
#define SPP_MSG_SET_REQUEST_ID              ((uint8_t)0xB2)
#define SPP_MSG_SET_REQUEST_BASE_SIZE       (4)
#define SPP_MSG_SET_RESPONSE_ID             ((uint8_t)0xB3)
#define SPP_MSG_SET_RESPONSE_SIZE           (2)
#define SPP_MSG_STREAM_REQUEST_ID           ((uint8_t)0xB4)
#define SPP_MSG_STREAM_REQUEST_SIZE         (6)
#define SPP_MSG_STREAM_RESPONSE_ID          ((uint8_t)0xB5)
#define SPP_MSG_STREAM_RESPONSE_SIZE        (3)
#define SPP_MSG_STREAM_ID                   ((uint8_t)0xB6)
#define SPP_MSG_STREAM_BASE_SIZE            (2)

/**
 * Property types
 */
#define SPP_PROP_T_U16          ((uint8_t)0xE0)
#define SPP_PROP_T_U32          ((uint8_t)0xE1)
#define SPP_PROP_T_I16          ((uint8_t)0xE2)
#define SPP_PROP_T_I32          ((uint8_t)0xE3)
#define SPP_PROP_T_FLOAT        ((uint8_t)0xE4)
#define SPP_PROP_T_BOOL         ((uint8_t)0xE5)
#define SPP_PROP_T_ARR          ((uint8_t)0xE6)

/**
 * Status
 */
#define SPP_STATUS_T                    uint8_t
#define SPP_STATUS_OK                   ((uint8_t)0x00)
#define SPP_STATUS_ERROR                ((uint8_t)0x01)
#define SPP_STATUS_BUSY                 ((uint8_t)0x02)
#define SPP_STATUS_PENDING_FULL         ((uint8_t)0x03)
#define SPP_STATUS_UNKNOWN_REQUEST      ((uint8_t)0x04)
#define SPP_STATUS_NO_SUCH_CLIENT       ((uint8_t)0x05)
#define SPP_STATUS_SHORT_MESSAGE        ((uint8_t)0x06)
#define SPP_STATUS_INVALID_SYNC_WORD    ((uint8_t)0x07)
#define SPP_STATUS_UNKNOWN_MSG_ID       ((uint8_t)0x07)
#define SPP_STATUS_OUT_OF_MEMORY        ((uint8_t)0x08)
#define SPP_STATUS_TOO_MANY_PROPERTIES  ((uint8_t)0x09)
#define SPP_STATUS_DUPLICATE_CLIENT     ((uint8_t)0x0A)
#define SPP_STATUS_UNKNOWN_PROPERTY     ((uint8_t)0x0B)

/**
 * Flags
 */
#define SPP_FLAG_READABLE           ((uint8_t)0x00)
#define SPP_FLAG_WRITEABLE          ((uint8_t)0x01)
#define SPP_FLAG_STREAMABLE_R       ((uint8_t)0x02)
#define SPP_FLAG_STREAMABLE_W       ((uint8_t)0x04)

/**
 * Streams
 */
#define SPP_STREAM_WRITE            ((uint8_t)0x10)
#define SPP_STREAM_READ             ((uint8_t)0x20)

/**
 * Protocol configuration
 */
#define SPP_MSG_SYNC_WORD               ((uint8_t)0xCD)
#define SPP_TOKEN_MAX_RESERVED          ((uint8_t)0xFF)
#define SPP_PROP_NAME_LENGTH_MAX        32
#define SPP_PENDING_REQUESTS_MAX        8
#define SPP_CONNECTIONS_MAX             5

#define SPP_HOST_ADDRESS_DEFAULT_IDX    0

#define SPP_STREAM_MAX_COUNT            16

#define SPP_STATE_READY                     ((uint8_t)0)
#define SPP_STATE_CONNECTING                ((uint8_t)1)

/**
 * macros
 */
#define MESSAGE_SIZE(body, address_length)      ((body) + SPP_MSG_HDR_SIZE(address_length))

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t SppState_t;

typedef struct SppAddress_s {
    void* address;
    uint16_t property_list_id;
} SppAddress_t;


typedef struct SppPropertyValue_s {
    uint8_t* value;
    uint8_t size;
} SppPropertyValue_t;

typedef union SppPropertyFlags_s {
    uint8_t value;
    struct {
        uint8_t is_readable         : 1;
        uint8_t is_writeable        : 1;
        uint8_t is_read_streamable  : 1;
        uint8_t is_write_streamable : 1;
    };
} SppPropertyFlags_t;

typedef struct SppPropertyDefinition_s {
    uint16_t id;
    uint8_t type;
    uint8_t size;
    SppPropertyFlags_t flags;
    char name[SPP_PROP_NAME_LENGTH_MAX];
    uint8_t name_length; // includes null terminator
} SppPropertyDefinition_t;

/**
 * Describes which property to stream at what rate
 */
typedef struct SppStream_s {
    SppPropertyDefinition_t *def;
    uint8_t stream_id;
    uint8_t stream_type;
    void* value;
    uint32_t period;
    uint32_t elapsed_time;
} SppStream_t;

#ifdef __cplusplus
}
#endif

#endif
