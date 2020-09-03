#ifndef __HTTP_PROTOCAL__
#define __HTTP_PROTOCAL__

enum DEVICE_MANAGEMENT_TYPE {
    SET_PASSWORD            = 0x0B01,
    GET_DEVICE_ID           = 0x0B02,
    SET_TIME 	            = 0x0B03,
    SET_DEVICE_RESTART      = 0x0B04,
    SET_DEVICE_RESTORE      = 0x0B05,
    SET_WIREDNETWORK        = 0x0B06,
    SET_WIFI_SETTING        = 0x0B07,
    SET_LOGO                = 0x0B08,
    SET_WALLPAPER           = 0x0B09,
    SET_DEFAULT_RECOG_MODE  = 0x0B0A,
    SET_GREETINGS           = 0x0B0B,
    SET_RECOG_CALLBACK      = 0x0B0C,
    SET_HEARTBEAT_CALLBACK  = 0x0B0D,
    SET_USERINFO_CALLBACK   = 0x0B0E,
    GET_SOFTWARE_VERSION    = 0x0B0F,

    // ATthis specific
    SET_PREVIEW_STREAM      = 0x0B80,
    SET_RGB_LED             = 0x0B81,
    SET_IR_LED              = 0x0B82,
    RESTORE_PASSWORD        = 0x0B83
};

enum USER_MANAGEMENT_TYPE {
    ADD_USER                = 0x0B01,
    DELETE_USER             = 0x0B03,
    GET_USER_INFO           = 0x0B06,
    SET_USER_AUTH_TIME      = 0x0B07,
    GET_USER_LIST           = 0x0B08,
    DELETE_USER_IMAGES      = 0x0B09,
    DELETE_DATASET          = 0x0B0A,
    // start user register
    SET_USER_REGISTER       = 0x0B92,
    GET_DATASET_RECORDS     = 0x0B93
};

enum RECOGNITION_MANAGEMENT_TYPE {
    GET_FEATURE                 = 0x0B01,
    GET_RECOG_MATCHING          = 0x0B02,
    GET_RECOG_INDENTIFICATION   = 0x0B03,
    SET_MULTIFACE_DETECT_NUM    = 0x0B04,
    SET_LIVENESS_TYPE           = 0x0B05,
    SET_QUALITY_CONTROL         = 0x0B06,
    SET_STRANGER                = 0x0B07,

    //Reserve 5 spots for each new algorithm
    SET_DMS_ENABLE              = 0x0B80,
    // detection params
    SET_MULTI_FACE_DETECTION_ENABLE = 0x0B85,
    SET_FACE_DETECTION_MINSIZE      = 0x0B86,
    SET_FACE_DETECTTION_BBOX        = 0x0B87,
    // recognition params
    SET_RECOGNITION_THRESHOLD       = 0x0B90,
    // liveness params
    SET_LIVENESS_THRESHOLD          = 0x0B91
};

enum RECORD_MANAGEMENT_TYPE {
    SEARCHING_RECORDS               = 0x0B01,
    DELETING_RECORD                 = 0x0B02,
    DELETING_ALL_RECORDS            = 0x0B03
};

#define HTTP_JSON_VALUE_TYPE_ERROR "wrong value type"

#endif
