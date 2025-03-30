#ifndef LIBLYNQ_AT_COMMON_H
#define LIBLYNQ_AT_COMMON_H 1
#ifdef __cplusplus
extern "C" {
#endif

typedef void ( *lynq_atsvc_incb )( const char *input,const int length);
typedef void ( *lynq_atsvc_outcb )(char *output, int out_max_size, int mode);
lynq_atsvc_incb lynq_register_at_common(lynq_atsvc_outcb out_cb);

#ifdef __cplusplus
}
#endif
#endif