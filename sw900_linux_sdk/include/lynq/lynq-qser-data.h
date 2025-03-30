#ifndef __LYNQ_QSER_DATA_H__
#define __LYNQ_QSER_DATA_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include <netinet/in.h>

typedef enum {
	QSER_DATA_CALL_ERROR_NONE = 0,
	QSER_DATA_CALL_ERROR_INVALID_PARAMS,
	QSER_DATA_CALL_ERROR_NO_INIT,
	QSER_DATA_CALL_ERROR_PDP_ACTIVATE,
	QSER_DATA_CALL_ERROR_PDP_NO_ACTIVATE,
	QSER_DATA_CALL_ERROR_IDX_NO_EXIST,
	QSER_DATA_CALL_ERROR_UNKNOWN,
} qser_data_call_error_e;

typedef enum {
	QSER_DATA_CALL_DISCONNECTED = 0,            /*!< call is disconnected */
	QSER_DATA_CALL_CONNECTED,                   /*!< call is connected */
} qser_data_call_state_e;

typedef enum {
	QSER_DATA_CALL_TYPE_IPV4 = 0,           /*!< IPv4 call. */
	QSER_DATA_CALL_TYPE_IPV6,               /*!< IPv6 call. */
	QSER_DATA_CALL_TYPE_IPV4V6,             /*!< IPv4 and IPv6 call (Only used call start or stop). */
} qser_data_call_ip_family_e;

typedef enum {
	QSER_APN_PDP_TYPE_IPV4 = 0,
	QSER_APN_PDP_TYPE_PPP,
	QSER_APN_PDP_TYPE_IPV6,
	QSER_APN_PDP_TYPE_IPV4V6,
} qser_apn_pdp_type_e;

typedef enum {
	QSER_APN_AUTH_PROTO_DEFAULT = 0,
	QSER_APN_AUTH_PROTO_NONE,
	QSER_APN_AUTH_PROTO_PAP,
	QSER_APN_AUTH_PROTO_CHAP,
	QSER_APN_AUTH_PROTO_PAP_CHAP,
} qser_apn_auth_proto_e;

#define QSER_APN_MAX_LIST 8
#define QSER_APN_NAME_SIZE 150+1
#define QSER_APN_USERNAME_SIZE 127+1
#define QSER_APN_PASSWORD_SIZE 127+1

struct v4_address_status {
	struct in_addr ip;                    /*!< Public IPv4 address. */
	struct in_addr gateway;               /*!< Public IPv4 gateway. */
	struct in_addr pri_dns;               /*!< Primary Domain Name Service IP address. */
	struct in_addr sec_dns;               /*!< Secondary Domain Name Service IP address. */
};

struct v6_address_status {
	struct in6_addr ip;                   /*!< Public IPv6 address. */
	struct in6_addr gateway;              /*!< Public IPv6 gateway. */
	struct in6_addr pri_dns;              /*!< Primary Domain Name Service IPv6 address. */
	struct in6_addr sec_dns;              /*!< Secondary Domain Name Service IPv6 address. */
};

typedef struct {
	char profile_idx;                               /*!< UMTS/CMDA profile ID. */
	char name[16];                                  /*!< Interface Name. */
	qser_data_call_ip_family_e ip_family;             /*!< IP version. */
	qser_data_call_state_e state;                     /*!< The dial status. */
	qser_data_call_error_e err;                       /*!< The Reason code after data call disconnected. */
	struct v4_address_status v4;            /*!< IPv4 information. */
	struct v6_address_status v6;            /*!< IPv6 information. */
} qser_data_call_state_s;

/*
 *!< Client callback function used to post event indications.
 */
typedef void (*qser_data_call_evt_cb_t)(qser_data_call_state_s *state);

typedef struct {
	char profile_idx;                        /*!< UMTS/CMDA profile ID. */
	bool reconnect;                          /*!< Whether to re-dial after disconnecting the network. */
	qser_data_call_ip_family_e ip_family;      /*!< IP version. */
	char cdma_username[QSER_APN_USERNAME_SIZE];     /*!< Username used during data network authentication. */
	char cdma_password[QSER_APN_PASSWORD_SIZE];     /*!< Password to be used during data network authentication. */
} qser_data_call_s;

struct pkt_stats {
	unsigned long pkts_tx;              /*!< Number of packets transmitted. */
	unsigned long pkts_rx;              /*!< Number of packets received. */
	long long bytes_tx;                 /*!< Number of bytes transmitted. */
	long long bytes_rx;                 /*!< Number of bytes received. */
	unsigned long pkts_dropped_tx;      /*!< Number of transmit packets dropped. */
	unsigned long pkts_dropped_rx;      /*!< Number of receive packets dropped. */
};

struct v4_info {
	char name[16];                          /*!< Interface Name. */
	qser_data_call_state_e state;             /*!< The dial status. */
	bool reconnect;                         /*!< re-dial flag. */
	struct v4_address_status addr;          /*!< IPv4 IP Address information. */
	struct pkt_stats stats;                 /*!< IPv4 statics */
};

struct v6_info {
	char name[16];                          /*!< Interface Name. */
	qser_data_call_state_e state;             /*!< The dial status. */
	bool reconnect;                         /*!< re-dial flag. */
	struct v6_address_status addr;          /*!< IPv6 IP Address information. */
	struct pkt_stats stats;                 /*!< IPv6 statics */
};

typedef struct {
	char profile_idx;                        /*!< UMTS/CDMA profile ID. */
	qser_data_call_ip_family_e ip_family;      /*!< IP version. */
	struct v4_info v4;                       /*!< IPv4 information */
	struct v6_info v6;                       /*!< IPv6 information */
} qser_data_call_info_s;

typedef struct {
	unsigned char profile_idx;              /*!< UMTS/CDMA profile ID. range: 0 - 7*/
	qser_apn_pdp_type_e pdp_type;             /*!< Packet Data Protocol (PDP) type specifies the type of data payload
	                                             exchanged over the airlink when the packet data session is
	                                             established with this profile. */ 
	qser_apn_auth_proto_e auth_proto;         /*!< Authentication Protocol. */
	char apn_name[QSER_APN_NAME_SIZE];        /*!< A string parameter that is a logical name used to select the GGSN
	                                             and external packet data network. */ 
	char username[QSER_APN_USERNAME_SIZE];    /*!< Username used during data network authentication. */
	char password[QSER_APN_PASSWORD_SIZE];    /*!< Password to be used during data network authentication. */
    char apn_type[QSER_APN_NAME_SIZE];
} qser_apn_info_s;

typedef struct {
	qser_apn_pdp_type_e pdp_type;             /*!< Packet Data Protocol (PDP) type specifies the type of data payload
						  exchanged over the airlink when the packet data session is
						  established with this profile. */ 
	qser_apn_auth_proto_e auth_proto;         /*!< Authentication Protocol. */
	char apn_name[QSER_APN_NAME_SIZE];        /*!< A string parameter that is a logical name used to select the GGSN
						  and external packet data network. */ 
	char username[QSER_APN_USERNAME_SIZE];    /*!< Username used during data network authentication. */
	char password[QSER_APN_PASSWORD_SIZE];    /*!< Password to be used during data network authentication. */
    char apn_type[QSER_APN_NAME_SIZE];
} qser_apn_add_s;

typedef struct {
	int cnt;
	qser_apn_info_s apn[QSER_APN_MAX_LIST];
} qser_apn_info_list_s;


/**
 * Initialization data call module, and callback function registered.
 *
 * @param [in] evt_cb           callback fucntion
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int qser_data_call_init(qser_data_call_evt_cb_t evt_cb);

/**
 * Destroy data call module, and unregister callback funciton
 *
 * @param 
 *   None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern void qser_data_call_destroy(void);

/**
 * Starts a data call. If profile index is zero, it will call CDMA profile.
 *
 * @param [in] data_call        The data call parameters
 * @param [out] error           Error code returned by data call 
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int qser_data_call_start(qser_data_call_s *data_call, qser_data_call_error_e *err);

/**
 * Starts a data call. If profile index is zero, it will call CDMA profile.
 *
 * @param [in] data_call        The data call parameters
 * @param [out] error           Error code returned by data call 
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int qser_data_call_start_async(qser_data_call_s *data_call, qser_data_call_error_e *err);

/**
 * Stop a data call.
 *
 * @param [in] profile_idx      UMTS/CDMA profile ID
 * @param [in] ip_family        IP Version
 * @param [out] error           Error code returned by data call 
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int qser_data_call_stop(char profile_idx, qser_data_call_ip_family_e ip_family, qser_data_call_error_e *err);

/**
 * Get a data call information.
 *
 * @param [in] profile_idx      UMTS/CDMA profile ID
 * @param [in] ip_family        IP Version
 * @param [out] info            The Data Call information
 * @param [out] error           Error code returned by data call 
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int qser_data_call_info_get(char profile_idx,
				 qser_data_call_ip_family_e ip_family,
				 qser_data_call_info_s *info,
				 qser_data_call_error_e *err);

/**
 * Changes the settings in a configured profile. 
 *
 * @param [in] apn      the profile information.
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned, such apn not exist.
 * 
 */
extern int qser_apn_set(qser_apn_info_s *apn);

/**
 * Retrieves the settings from a configured profile.
 *
 * @param [in] profile_idx        UMTS/CDMA profile ID
 * @param [out] apn               the profile information.
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int qser_apn_get(unsigned char profile_idx, qser_apn_info_s *apn);

/**
 * Retrieves the settings from a configured profile.
 *
 * @param [in] apn               the profile information.
 * @param [out] profile_idx        UMTS/CDMA profile ID
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int qser_apn_add(qser_apn_add_s *apn, unsigned char *profile_idx);

/**
 * Delete a configured profile.
 *
 * @param [in] profile_idx        UMTS/CDMA profile ID
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int qser_apn_del(unsigned char profile_idx);

/**
 * Retrieves the settings from a configured profile list.
 *
 * @param [out] apn_list          the profile list information.
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int qser_apn_get_list(qser_apn_info_list_s *apn_list);
#ifdef __cplusplus
}
#endif
#endif

