#ifndef LYNQ_DATA_H
#define LYNQ_DATA_H 
#ifdef __cplusplus
extern "C" {
#endif
#define LYNQ_APN_CHANNEL_MAX 10
#define LYNQ_PDP_TYPE_MAX_LEN 16
#define LYNQ_IFACE_NAME_MAX_LEN 50
#define LYNQ_APN_MAX_LEN 50
#define LYNQ_TYPE_MAX_LEN 50
#define LYNQ_ADDRESSES_MAX_LEN 100
#define LYNQ_DNSES_MAX_LEN 256
#define LYNQ_GATEWAYS_MAX_LEN 50
#define LYNQ_PCSCF_MAX_LEN 256
#define LYNQ_APN_TYPE_MAX_LEN 50
#define LYNQ_PDP_ADDR_MAX_LEN 64
#define LYNQ_DNS_ADDR_MAX_LEN 256
#define LYNQ_GETWAYS_ADDR_MAX_LEN 64
#define LYNQ_POXY_ADDR_MAX_LEN 64


typedef struct {
    int            status;     /* A RIL_DataCallFailCause, 0 which is PDP_FAIL_NONE if no error */
    int            suggestedRetryTime; /* If status != 0, this fields indicates the suggested retry
                                           back-off timer value RIL wants to override the one
                                           pre-configured in FW.
                                           The unit is miliseconds.
                                           The value < 0 means no value is suggested.
                                           The value 0 means retry should be done ASAP.
                                           The value of INT_MAX(0x7fffffff) means no retry. */
    int            cid;        /* Context ID, uniquely identifies this call */
    int            active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    char           type[LYNQ_PDP_TYPE_MAX_LEN];       /* One of the PDP_type values in TS 27.007 section 10.1.1.
                                   For example, "IP", "IPV6", "IPV4V6", or "PPP". If status is
                                   PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED this is the type supported
                                   such as "IP" or "IPV6" */
    char           ifname[LYNQ_IFACE_NAME_MAX_LEN];     /* The network interface name */
    char           addresses[LYNQ_PDP_ADDR_MAX_LEN];  /* A space-delimited list of addresses with optional "/" prefix length,
                                   e.g., "192.0.1.3" or "192.0.1.11/16 2001:db8::1/64".
                                   May not be empty, typically 1 IPv4 or 1 IPv6 or
                                   one of each. If the prefix length is absent the addresses
                                   are assumed to be point to point with IPv4 having a prefix
                                   length of 32 and IPv6 128. */
    char           dnses[LYNQ_DNS_ADDR_MAX_LEN];      /* A space-delimited list of DNS server addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty. */
    char           gateways[LYNQ_GETWAYS_ADDR_MAX_LEN];   /* A space-delimited list of default gateway addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty in which case the addresses represent point
                                   to point connections. */
    char           pcscf[LYNQ_POXY_ADDR_MAX_LEN];    /* the Proxy Call State Control Function address
                                 via PCO(Protocol Configuration Option) for IMS client. */
    int            mtu;        /* MTU received from network
                                   Value <= 0 means network has either not sent a value or
                                   sent an invalid value */
} lynq_data_call_response_v11_t;

typedef struct
{
    int index;
    char apn[LYNQ_APN_MAX_LEN];
    char apnType[LYNQ_APN_TYPE_MAX_LEN];
}lynq_apn_info;

int lynq_init_data(int uToken);
int lynq_deinit_data();
int lynq_setup_data_call(int *handle);
int lynq_deactive_data_call(int *handle);
int lynq_setup_data_call_sp(int *handle,char *apn,char *apnType,char *user,char *password,char *authType,char *normalProtocol,char *roamingProtocol);
int lynq_setup_data_call_sp_t106(int *handle,char *apn,char *apnType,char *user,char *password,char *authType,char *normalProtocol,char *roamingProtocol);
//int lynq_deactive_data_call_sp(int *handle,char *apnType);
int lynq_get_data_call_list(int *handle,lynq_data_call_response_v11_t *dataCallList);
int lynq_wait_data_call_state_change(int *handle);
int lynq_modify_apn_db(const int cmd,char *id,char *mcc,char *mnc,char *apn,char *apntype,char *user,char *password,char *normalprotocol,char *roamingprotocol,char *carrier,char *out);
int lynq_reset_apn(char *result);
int lynq_get_apn_table(int *size,lynq_apn_info **list);
void lynq_release_wait_data_call();

#ifdef __cplusplus
}
#endif
#endif
