typedef enum 
{
    E_QSER_VOICE_CALL_STATE_INCOMING      = 0x0000, /**< MT incoming; CC setup. */
    E_QSER_VOICE_CALL_STATE_DIALING       = 0x0001, /**< Dialing state. */
    E_QSER_VOICE_CALL_STATE_ALERTING      = 0x0002, /**< MT call waiting; MO alterting. */
    E_QSER_VOICE_CALL_STATE_ACTIVE        = 0x0003, /**< Call is active. */
    E_QSER_VOICE_CALL_STATE_HOLDING       = 0x0004, /**< Call is on hold. */
    E_QSER_VOICE_CALL_STATE_END           = 0x0005, /**< Call is disconnected. */
    E_QSER_VOICE_CALL_STATE_WAITING       = 0x0006, /**< Call is waiting. */
}qser_voice_call_state_t;

typedef enum
{
    E_QSER_VCALL_EXTERNAL_SLOT_1,
    E_QSER_VCALL_EXTERNAL_SLOT_2,
    E_QSER_VCALL_EMBEDDED,
    E_QSER_VCALL_REMOTE,
    E_QSER_VCALL_ID_MAX
}E_QSER_VCALL_ID_T;


typedef qser_voice_call_state_t E_QSER_VOICE_CALL_STATE_T;

typedef void (*QSER_VoiceCall_StateHandlerFunc_t)
(
    int                     caLOCAL_C_INCLUDESll_id,
    char*                   phone_num,
    E_QSER_VOICE_CALL_STATE_T state,
    void                    *contextPtr
);

