/*
* MBTK Network Interface Control Header.
*
* Author : lb
* Date   : 2021/8/20 13:18:00
*
*/
#ifndef _MBTK_IFC_H
#define _MBTK_IFC_H
#include <netinet/in.h>

int mbtk_ifc_open(void);
int mbtk_ifc_close(void);
int mbtk_ifc_up(const char *name);
int mbtk_ifc_down(const char *name);
int mbtk_ifc_set_addr(const char *name, in_addr_t addr, in_addr_t netmask);
int mbtk_ifc_get_addr(const char *name, void *addr);
int mbtk_ifc_get_hwaddr(const char *name, void *ptr);
int mbtk_ifc_get_ifindex(const char *name, int *if_indexp);
int mbtk_ifc_ip_config(const char *ifname, const char *ipv4, const char *mask, const char *gateway);
int mbtk_ifc_set_netmask(const char *ifname, const char *netmask);

int mbtk_ifc_configure1(const char *ifname,
                        in_addr_t address,
                        uint32_t prefixLength,
                        in_addr_t gateway,
                        in_addr_t netmask);

int mbtk_ifc_configure2(const char *ifname,
                        const char *ipv4,
                        uint32_t prefixLength,
                        const char *gateway,
                        const char *netmask);

int mbtk_ipv6_config(const char *ifname, const char *ipv6, uint32_t prefixLength);

#endif /* _MBTK_IFC_H */

