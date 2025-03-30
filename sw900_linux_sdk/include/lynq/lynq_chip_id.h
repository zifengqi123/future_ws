/**@file lynq-systime.h
 *
 * @brief Sync systime form each time source.
 *
 * @author sj.zhang
 *
 * @date 2023-08-14
 *
 * @version V1.0
 *
 * @copyright Copyright (c) MobileTek
 */

#ifndef _LYNQ_CHIP_ID_H_
#define _LYNQ_CHIP_ID_H_

#ifdef __cplusplus
extern "C"
{
#endif
/**@brief get chip id.
*
* @param void
*
* @return
*      0:success
*      other:failure
*/
int lynq_get_chip_id(char *chip_id);

#ifdef __cplusplus
}
#endif
#endif
