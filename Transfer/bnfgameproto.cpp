#include "bnfgameproto.h"

CBnFGameProto g_BnFGameProto;
CBnFGameProto g_BnFGameProtoDe;
CBnFGameProto g_BnFGameProtoEN;

CBnFGameProto::CBnFGameProto()
{
    m_vMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}
CBnFGameProto::~CBnFGameProto()
{
}
int CBnFGameProto::Encode(int iMsgID, void *pHost, char *pNet, int iNetSize)
{
	m_oData.Prepare(pNet, iNetSize);
	EnFuncCBnFGameProto pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;
	return (*pfnEncode)(pHost, &m_oData);
}
int CBnFGameProto::Decode(int iMsgID, char *pNet,int iNetSize, void *pHost, int iHostSize)
{
	m_oData.Prepare(pNet, iNetSize);
	if(iHostSize < m_iMaxPkgSize)
		return -1;
	DeFuncCBnFGameProto pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;
	return (*pfnDecode)(pHost, &m_oData);
}
EnFuncCBnFGameProto CBnFGameProto::FindEncodeFunc(int iMsgID)
{
	if(iMsgID - BNFGAMEPROTO_MSGID_OFFSET >= BNFGAMEPROTO_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - BNFGAMEPROTO_MSGID_OFFSET];
}
DeFuncCBnFGameProto CBnFGameProto::FindDecodeFunc(int iMsgID)
{
	if(iMsgID - BNFGAMEPROTO_MSGID_OFFSET >= BNFGAMEPROTO_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - BNFGAMEPROTO_MSGID_OFFSET];
}
bool CBnFGameProto::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[MSG_LOGIN_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_LOGIN_REQ;
	m_DecodeFuncArray[MSG_LOGIN_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_LOGIN_REQ;
	m_vMsgList.push_back(MSG_LOGIN_REQ);
	if( (int)sizeof(CLI_GS_LOGIN_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_LOGIN_REQ);

	m_EncodeFuncArray[MSG_LOGIN_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_LOGIN_ACK;
	m_DecodeFuncArray[MSG_LOGIN_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_LOGIN_ACK;
	m_vMsgList.push_back(MSG_LOGIN_ACK);
	if( (int)sizeof(GS_CLI_LOGIN_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_LOGIN_ACK);

	m_EncodeFuncArray[MSG_PING_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_PING_REQ;
	m_DecodeFuncArray[MSG_PING_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_PING_REQ;
	m_vMsgList.push_back(MSG_PING_REQ);
	if( (int)sizeof(CLI_GS_PING_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_PING_REQ);

	m_EncodeFuncArray[MSG_PING_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_PING_ACK;
	m_DecodeFuncArray[MSG_PING_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_PING_ACK;
	m_vMsgList.push_back(MSG_PING_ACK);
	if( (int)sizeof(GS_CLI_PING_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_PING_ACK);

	m_EncodeFuncArray[MSG_MOVE_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_MOVE_REQ;
	m_DecodeFuncArray[MSG_MOVE_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_MOVE_REQ;
	m_vMsgList.push_back(MSG_MOVE_REQ);
	if( (int)sizeof(CLI_GS_MOVE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_MOVE_REQ);

	m_EncodeFuncArray[MSG_MOVE_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_MOVE_NTF;
	m_DecodeFuncArray[MSG_MOVE_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_MOVE_NTF;
	m_vMsgList.push_back(MSG_MOVE_NTF);
	if( (int)sizeof(GS_CLI_MOVE_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_MOVE_NTF);

	m_EncodeFuncArray[MSG_MOVE_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_MOVE_ACK;
	m_DecodeFuncArray[MSG_MOVE_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_MOVE_ACK;
	m_vMsgList.push_back(MSG_MOVE_ACK);
	if( (int)sizeof(GS_CLI_MOVE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_MOVE_ACK);

	m_EncodeFuncArray[MSG_PROPERTYCHANGED_INT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_PROPERTYCHANGED_INT_NTF;
	m_DecodeFuncArray[MSG_PROPERTYCHANGED_INT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_PROPERTYCHANGED_INT_NTF;
	m_vMsgList.push_back(MSG_PROPERTYCHANGED_INT_NTF);
	if( (int)sizeof(GS_CLI_PROPERTYCHANGED_INT_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_PROPERTYCHANGED_INT_NTF);

	m_EncodeFuncArray[MSG_PROPERTYCHANGED_UINT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_PROPERTYCHANGED_UINT_NTF;
	m_DecodeFuncArray[MSG_PROPERTYCHANGED_UINT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_PROPERTYCHANGED_UINT_NTF;
	m_vMsgList.push_back(MSG_PROPERTYCHANGED_UINT_NTF);
	if( (int)sizeof(GS_CLI_PROPERTYCHANGED_UINT_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_PROPERTYCHANGED_UINT_NTF);

	m_EncodeFuncArray[MSG_PROPERTYCHANGED_FLOAT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_PROPERTYCHANGED_FLOAT_NTF;
	m_DecodeFuncArray[MSG_PROPERTYCHANGED_FLOAT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_PROPERTYCHANGED_FLOAT_NTF;
	m_vMsgList.push_back(MSG_PROPERTYCHANGED_FLOAT_NTF);
	if( (int)sizeof(GS_CLI_PROPERTYCHANGED_FLOAT_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_PROPERTYCHANGED_FLOAT_NTF);

	m_EncodeFuncArray[MSG_FORCE_REPOSITIONED_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_FORCE_REPOSITIONED_NTF;
	m_DecodeFuncArray[MSG_FORCE_REPOSITIONED_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_FORCE_REPOSITIONED_NTF;
	m_vMsgList.push_back(MSG_FORCE_REPOSITIONED_NTF);
	if( (int)sizeof(GS_CLI_FORCE_REPOSITIONED_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_FORCE_REPOSITIONED_NTF);

	m_EncodeFuncArray[MSG_CREATURE_PROPERTIES_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_CREATURE_PROPERTIES_REQ;
	m_DecodeFuncArray[MSG_CREATURE_PROPERTIES_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_CREATURE_PROPERTIES_REQ;
	m_vMsgList.push_back(MSG_CREATURE_PROPERTIES_REQ);
	if( (int)sizeof(CLI_GS_CREATURE_PROPERTIES_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_CREATURE_PROPERTIES_REQ);

	m_EncodeFuncArray[MSG_CREATURE_PROPERTIES_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_CREATURE_PROPERTIES_ACK;
	m_DecodeFuncArray[MSG_CREATURE_PROPERTIES_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_CREATURE_PROPERTIES_ACK;
	m_vMsgList.push_back(MSG_CREATURE_PROPERTIES_ACK);
	if( (int)sizeof(GS_CLI_CREATURE_PROPERTIES_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_CREATURE_PROPERTIES_ACK);

	m_EncodeFuncArray[MSG_JUMP_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_JUMP_NTF;
	m_DecodeFuncArray[MSG_JUMP_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_JUMP_NTF;
	m_vMsgList.push_back(MSG_JUMP_NTF);
	if( (int)sizeof(GS_CLI_JUMP_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_JUMP_NTF);

	m_EncodeFuncArray[MSG_SKILL_PROPERTIES_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_SKILL_PROPERTIES_NTF;
	m_DecodeFuncArray[MSG_SKILL_PROPERTIES_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_SKILL_PROPERTIES_NTF;
	m_vMsgList.push_back(MSG_SKILL_PROPERTIES_NTF);
	if( (int)sizeof(GS_CLI_SKILL_PROPERTIES_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_SKILL_PROPERTIES_NTF);

	m_EncodeFuncArray[MSG_CASTING_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_CASTING_REQ;
	m_DecodeFuncArray[MSG_CASTING_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_CASTING_REQ;
	m_vMsgList.push_back(MSG_CASTING_REQ);
	if( (int)sizeof(CLI_GS_CASTING_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_CASTING_REQ);

	m_EncodeFuncArray[MSG_CASTING_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_CASTING_NTF;
	m_DecodeFuncArray[MSG_CASTING_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_CASTING_NTF;
	m_vMsgList.push_back(MSG_CASTING_NTF);
	if( (int)sizeof(GS_CLI_CASTING_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_CASTING_NTF);

	m_EncodeFuncArray[MSG_CASTED_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_CASTED_NTF;
	m_DecodeFuncArray[MSG_CASTED_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_CASTED_NTF;
	m_vMsgList.push_back(MSG_CASTED_NTF);
	if( (int)sizeof(GS_CLI_CASTED_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_CASTED_NTF);

	m_EncodeFuncArray[MSG_CASTING_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_CASTING_ACK;
	m_DecodeFuncArray[MSG_CASTING_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_CASTING_ACK;
	m_vMsgList.push_back(MSG_CASTING_ACK);
	if( (int)sizeof(GS_CLI_CASTING_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_CASTING_ACK);

	m_EncodeFuncArray[MSG_SKILL_COOLDOWN_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_SKILL_COOLDOWN_NTF;
	m_DecodeFuncArray[MSG_SKILL_COOLDOWN_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_SKILL_COOLDOWN_NTF;
	m_vMsgList.push_back(MSG_SKILL_COOLDOWN_NTF);
	if( (int)sizeof(GS_CLI_SKILL_COOLDOWN_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_SKILL_COOLDOWN_NTF);

	m_EncodeFuncArray[MSG_PASSIVE_SKILL_ACTIVE - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_PASSIVE_SKILL_ACTIVE;
	m_DecodeFuncArray[MSG_PASSIVE_SKILL_ACTIVE - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_PASSIVE_SKILL_ACTIVE;
	m_vMsgList.push_back(MSG_PASSIVE_SKILL_ACTIVE);
	if( (int)sizeof(GS_CLI_PASSIVE_SKILL_ACTIVE) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_PASSIVE_SKILL_ACTIVE);

	m_EncodeFuncArray[MSG_CREATURE_APPEAR_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_CREATURE_APPEAR_NTF;
	m_DecodeFuncArray[MSG_CREATURE_APPEAR_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_CREATURE_APPEAR_NTF;
	m_vMsgList.push_back(MSG_CREATURE_APPEAR_NTF);
	if( (int)sizeof(GS_CLI_CREATURE_APPEAR_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_CREATURE_APPEAR_NTF);

	m_EncodeFuncArray[MSG_CREATURE_DISAPPEAR_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_CREATURE_DISAPPEAR_NTF;
	m_DecodeFuncArray[MSG_CREATURE_DISAPPEAR_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_CREATURE_DISAPPEAR_NTF;
	m_vMsgList.push_back(MSG_CREATURE_DISAPPEAR_NTF);
	if( (int)sizeof(GS_CLI_CREATURE_DISAPPEAR_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_CREATURE_DISAPPEAR_NTF);

	m_EncodeFuncArray[MSG_CREATURE_ON_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_CREATURE_ON_NTF;
	m_DecodeFuncArray[MSG_CREATURE_ON_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_CREATURE_ON_NTF;
	m_vMsgList.push_back(MSG_CREATURE_ON_NTF);
	if( (int)sizeof(GS_CLI_CREATURE_ON_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_CREATURE_ON_NTF);

	m_EncodeFuncArray[MSG_CREATURE_OFF_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_CREATURE_OFF_NTF;
	m_DecodeFuncArray[MSG_CREATURE_OFF_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_CREATURE_OFF_NTF;
	m_vMsgList.push_back(MSG_CREATURE_OFF_NTF);
	if( (int)sizeof(GS_CLI_CREATURE_OFF_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_CREATURE_OFF_NTF);

	m_EncodeFuncArray[MSG_OPERATOR_APPEAR_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_OPERATOR_APPEAR_NTF;
	m_DecodeFuncArray[MSG_OPERATOR_APPEAR_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_OPERATOR_APPEAR_NTF;
	m_vMsgList.push_back(MSG_OPERATOR_APPEAR_NTF);
	if( (int)sizeof(GS_CLI_OPERATOR_APPEAR_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_OPERATOR_APPEAR_NTF);

	m_EncodeFuncArray[MSG_OPERATOR_DISAPPEAR_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_OPERATOR_DISAPPEAR_NTF;
	m_DecodeFuncArray[MSG_OPERATOR_DISAPPEAR_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_OPERATOR_DISAPPEAR_NTF;
	m_vMsgList.push_back(MSG_OPERATOR_DISAPPEAR_NTF);
	if( (int)sizeof(GS_CLI_OPERATOR_DISAPPEAR_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_OPERATOR_DISAPPEAR_NTF);

	m_EncodeFuncArray[MSG_ADD_BUFF_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_ADD_BUFF_NTF;
	m_DecodeFuncArray[MSG_ADD_BUFF_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_ADD_BUFF_NTF;
	m_vMsgList.push_back(MSG_ADD_BUFF_NTF);
	if( (int)sizeof(GS_CLI_ADD_BUFF_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_ADD_BUFF_NTF);

	m_EncodeFuncArray[MSG_UPDATE_BUFF_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_UPDATE_BUFF_NTF;
	m_DecodeFuncArray[MSG_UPDATE_BUFF_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_UPDATE_BUFF_NTF;
	m_vMsgList.push_back(MSG_UPDATE_BUFF_NTF);
	if( (int)sizeof(GS_CLI_UPDATE_BUFF_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_UPDATE_BUFF_NTF);

	m_EncodeFuncArray[MSG_DEL_BUFF_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_DEL_BUFF_NTF;
	m_DecodeFuncArray[MSG_DEL_BUFF_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_DEL_BUFF_NTF;
	m_vMsgList.push_back(MSG_DEL_BUFF_NTF);
	if( (int)sizeof(GS_CLI_DEL_BUFF_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_DEL_BUFF_NTF);

	m_EncodeFuncArray[MSG_MOVE_EQUIPMENT_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_MOVE_EQUIPMENT_REQ;
	m_DecodeFuncArray[MSG_MOVE_EQUIPMENT_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_MOVE_EQUIPMENT_REQ;
	m_vMsgList.push_back(MSG_MOVE_EQUIPMENT_REQ);
	if( (int)sizeof(CLI_GS_MOVE_EQUIPMENT_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_MOVE_EQUIPMENT_REQ);

	m_EncodeFuncArray[MSG_MOVE_EQUIPMENT_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_MOVE_EQUIPMENT_ACK;
	m_DecodeFuncArray[MSG_MOVE_EQUIPMENT_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_MOVE_EQUIPMENT_ACK;
	m_vMsgList.push_back(MSG_MOVE_EQUIPMENT_ACK);
	if( (int)sizeof(GS_CLI_MOVE_EQUIPMENT_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_MOVE_EQUIPMENT_ACK);

	m_EncodeFuncArray[MSG_MOVE_EQUIPMENT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_MOVE_EQUIPMENT_NTF;
	m_DecodeFuncArray[MSG_MOVE_EQUIPMENT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_MOVE_EQUIPMENT_NTF;
	m_vMsgList.push_back(MSG_MOVE_EQUIPMENT_NTF);
	if( (int)sizeof(GS_CLI_MOVE_EQUIPMENT_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_MOVE_EQUIPMENT_NTF);

	m_EncodeFuncArray[MSG_DROP_EQUIPMENT_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_DROP_EQUIPMENT_REQ;
	m_DecodeFuncArray[MSG_DROP_EQUIPMENT_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_DROP_EQUIPMENT_REQ;
	m_vMsgList.push_back(MSG_DROP_EQUIPMENT_REQ);
	if( (int)sizeof(CLI_GS_DROP_EQUIPMENT_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_DROP_EQUIPMENT_REQ);

	m_EncodeFuncArray[MSG_DROP_EQUIPMENT_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_DROP_EQUIPMENT_ACK;
	m_DecodeFuncArray[MSG_DROP_EQUIPMENT_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_DROP_EQUIPMENT_ACK;
	m_vMsgList.push_back(MSG_DROP_EQUIPMENT_ACK);
	if( (int)sizeof(GS_CLI_DROP_EQUIPMENT_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_DROP_EQUIPMENT_ACK);

	m_EncodeFuncArray[MSG_USE_CONSUME_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_USE_CONSUME_REQ;
	m_DecodeFuncArray[MSG_USE_CONSUME_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_USE_CONSUME_REQ;
	m_vMsgList.push_back(MSG_USE_CONSUME_REQ);
	if( (int)sizeof(CLI_GS_USE_CONSUME_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_USE_CONSUME_REQ);

	m_EncodeFuncArray[MSG_USE_CONSUME_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_USE_CONSUME_ACK;
	m_DecodeFuncArray[MSG_USE_CONSUME_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_USE_CONSUME_ACK;
	m_vMsgList.push_back(MSG_USE_CONSUME_ACK);
	if( (int)sizeof(GS_CLI_USE_CONSUME_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_USE_CONSUME_ACK);

	m_EncodeFuncArray[MSG_UPDATED_EQUIPMENT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_UPDATED_EQUIPMENT_NTF;
	m_DecodeFuncArray[MSG_UPDATED_EQUIPMENT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_UPDATED_EQUIPMENT_NTF;
	m_vMsgList.push_back(MSG_UPDATED_EQUIPMENT_NTF);
	if( (int)sizeof(GS_CLI_UPDATED_EQUIPMENT_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_UPDATED_EQUIPMENT_NTF);

	m_EncodeFuncArray[MSG_LIST_EQUIPMENT_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_LIST_EQUIPMENT_REQ;
	m_DecodeFuncArray[MSG_LIST_EQUIPMENT_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_LIST_EQUIPMENT_REQ;
	m_vMsgList.push_back(MSG_LIST_EQUIPMENT_REQ);
	if( (int)sizeof(CLI_GS_LIST_EQUIPMENT_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_LIST_EQUIPMENT_REQ);

	m_EncodeFuncArray[MSG_LIST_EQUIPMENT_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_LIST_EQUIPMENT_ACK;
	m_DecodeFuncArray[MSG_LIST_EQUIPMENT_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_LIST_EQUIPMENT_ACK;
	m_vMsgList.push_back(MSG_LIST_EQUIPMENT_ACK);
	if( (int)sizeof(GS_CLI_LIST_EQUIPMENT_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_LIST_EQUIPMENT_ACK);

	m_EncodeFuncArray[MSG_LIST_EQUIPMENT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_LIST_EQUIPMENT_NTF;
	m_DecodeFuncArray[MSG_LIST_EQUIPMENT_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_LIST_EQUIPMENT_NTF;
	m_vMsgList.push_back(MSG_LIST_EQUIPMENT_NTF);
	if( (int)sizeof(GS_CLI_LIST_EQUIPMENT_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_LIST_EQUIPMENT_NTF);

	m_EncodeFuncArray[MSG_SIMPLE_MESSAGE_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_SIMPLE_MESSAGE_NTF;
	m_DecodeFuncArray[MSG_SIMPLE_MESSAGE_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_SIMPLE_MESSAGE_NTF;
	m_vMsgList.push_back(MSG_SIMPLE_MESSAGE_NTF);
	if( (int)sizeof(GS_CLI_SIMPLE_MESSAGE_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_SIMPLE_MESSAGE_NTF);

	m_EncodeFuncArray[MSG_COMMAND_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_COMMAND_REQ;
	m_DecodeFuncArray[MSG_COMMAND_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_COMMAND_REQ;
	m_vMsgList.push_back(MSG_COMMAND_REQ);
	if( (int)sizeof(CLI_GS_COMMAND_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_COMMAND_REQ);

	m_EncodeFuncArray[MSG_COMMAND_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_COMMAND_ACK;
	m_DecodeFuncArray[MSG_COMMAND_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_COMMAND_ACK;
	m_vMsgList.push_back(MSG_COMMAND_ACK);
	if( (int)sizeof(GS_CLI_COMMAND_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_COMMAND_ACK);

	m_EncodeFuncArray[MSG_QUEST_RESULT_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_QUEST_RESULT_REQ;
	m_DecodeFuncArray[MSG_QUEST_RESULT_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_QUEST_RESULT_REQ;
	m_vMsgList.push_back(MSG_QUEST_RESULT_REQ);
	if( (int)sizeof(CLI_GS_QUEST_RESULT_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_QUEST_RESULT_REQ);

	m_EncodeFuncArray[MSG_QUERY_OPERATOR_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_QUERY_OPERATOR_REQ;
	m_DecodeFuncArray[MSG_QUERY_OPERATOR_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_QUERY_OPERATOR_REQ;
	m_vMsgList.push_back(MSG_QUERY_OPERATOR_REQ);
	if( (int)sizeof(CLI_GS_QUERY_OPERATOR_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_QUERY_OPERATOR_REQ);

	m_EncodeFuncArray[MSG_CHANGE_TABLE_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_CHANGE_TABLE_REQ;
	m_DecodeFuncArray[MSG_CHANGE_TABLE_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_CHANGE_TABLE_REQ;
	m_vMsgList.push_back(MSG_CHANGE_TABLE_REQ);
	if( (int)sizeof(CLI_GS_CHANGE_TABLE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_CHANGE_TABLE_REQ);

	m_EncodeFuncArray[MSG_CHANGE_TABLE_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_CHANGE_TABLE_ACK;
	m_DecodeFuncArray[MSG_CHANGE_TABLE_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_CHANGE_TABLE_ACK;
	m_vMsgList.push_back(MSG_CHANGE_TABLE_ACK);
	if( (int)sizeof(GS_CLI_CHANGE_TABLE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_CHANGE_TABLE_ACK);

	m_EncodeFuncArray[MSG_REMOVE_QTE_BUFF_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_REMOVE_QTE_BUFF_REQ;
	m_DecodeFuncArray[MSG_REMOVE_QTE_BUFF_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_REMOVE_QTE_BUFF_REQ;
	m_vMsgList.push_back(MSG_REMOVE_QTE_BUFF_REQ);
	if( (int)sizeof(CLI_GS_REMOVE_QTE_BUFF_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_REMOVE_QTE_BUFF_REQ);

	m_EncodeFuncArray[MSG_REMOVE_QTE_BUFF_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_REMOVE_QTE_BUFF_ACK;
	m_DecodeFuncArray[MSG_REMOVE_QTE_BUFF_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_REMOVE_QTE_BUFF_ACK;
	m_vMsgList.push_back(MSG_REMOVE_QTE_BUFF_ACK);
	if( (int)sizeof(GS_CLI_REMOVE_QTE_BUFF_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_REMOVE_QTE_BUFF_ACK);

	m_EncodeFuncArray[MSG_LOCK_PLAYER_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_LOCK_PLAYER_REQ;
	m_DecodeFuncArray[MSG_LOCK_PLAYER_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_LOCK_PLAYER_REQ;
	m_vMsgList.push_back(MSG_LOCK_PLAYER_REQ);
	if( (int)sizeof(CLI_GS_LOCK_PLAYER_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_LOCK_PLAYER_REQ);

	m_EncodeFuncArray[MSG_WINNER_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_WINNER_NTF;
	m_DecodeFuncArray[MSG_WINNER_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_WINNER_NTF;
	m_vMsgList.push_back(MSG_WINNER_NTF);
	if( (int)sizeof(GS_CLI_WINNER_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_WINNER_NTF);

	m_EncodeFuncArray[MSG_SPEAK_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_SPEAK_NTF;
	m_DecodeFuncArray[MSG_SPEAK_NTF - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_SPEAK_NTF;
	m_vMsgList.push_back(MSG_SPEAK_NTF);
	if( (int)sizeof(GS_CLI_SPEAK_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_SPEAK_NTF);

	m_EncodeFuncArray[MSG_TALK_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_TALK_REQ;
	m_DecodeFuncArray[MSG_TALK_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_TALK_REQ;
	m_vMsgList.push_back(MSG_TALK_REQ);
	if( (int)sizeof(CLI_GS_TALK_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_TALK_REQ);

	m_EncodeFuncArray[MSG_TALK_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_TALK_ACK;
	m_DecodeFuncArray[MSG_TALK_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_TALK_ACK;
	m_vMsgList.push_back(MSG_TALK_ACK);
	if( (int)sizeof(GS_CLI_TALK_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_TALK_ACK);

	m_EncodeFuncArray[MSG_NOTIFY_JSON - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_NOTIFY_ACK;
	m_DecodeFuncArray[MSG_NOTIFY_JSON - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_NOTIFY_ACK;
	m_vMsgList.push_back(MSG_NOTIFY_JSON);
	if( (int)sizeof(GS_CLI_NOTIFY_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_NOTIFY_ACK);

	m_EncodeFuncArray[MSG_JUMP_SCENE_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeCLI_GS_JUMP_SCENE_REQ;
	m_DecodeFuncArray[MSG_JUMP_SCENE_REQ - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeCLI_GS_JUMP_SCENE_REQ;
	m_vMsgList.push_back(MSG_JUMP_SCENE_REQ);
	if( (int)sizeof(CLI_GS_JUMP_SCENE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(CLI_GS_JUMP_SCENE_REQ);

	m_EncodeFuncArray[MSG_JUMP_SCENE_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &EncodeGS_CLI_JUMP_SCENE_ACK;
	m_DecodeFuncArray[MSG_JUMP_SCENE_ACK - BNFGAMEPROTO_MSGID_OFFSET] = &DecodeGS_CLI_JUMP_SCENE_ACK;
	m_vMsgList.push_back(MSG_JUMP_SCENE_ACK);
	if( (int)sizeof(GS_CLI_JUMP_SCENE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (int)sizeof(GS_CLI_JUMP_SCENE_ACK);

	return true;
}

int EncodeCLI_GS_JUMP_SCENE_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_JUMP_SCENE_REQ *pstIn = (CLI_GS_JUMP_SCENE_REQ*)pHost;


	if(-1 == poNetData->AddString(pstIn->Host, sizeof(pstIn->Host)))

		return -1;


	if(-1 == poNetData->AddString(pstIn->UserId, sizeof(pstIn->UserId)))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Port))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_JUMP_SCENE_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_JUMP_SCENE_REQ *pstOut = (CLI_GS_JUMP_SCENE_REQ*)pHost;


	if(-1 == poNetData->DelString(pstOut->Host, sizeof(pstOut->Host)))

		return -1;


	if(-1 == poNetData->DelString(pstOut->UserId, sizeof(pstOut->UserId)))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Port))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_JUMP_SCENE_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_JUMP_SCENE_ACK *pstIn = (GS_CLI_JUMP_SCENE_ACK*)pHost;


	if(-1 == poNetData->AddInt(pstIn->ErrCode))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Id))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_JUMP_SCENE_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_JUMP_SCENE_ACK *pstOut = (GS_CLI_JUMP_SCENE_ACK*)pHost;


	if(-1 == poNetData->DelInt(pstOut->ErrCode))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Id))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_LOGIN_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_LOGIN_REQ *pstIn = (CLI_GS_LOGIN_REQ*)pHost;


	if(-1 == poNetData->AddString(pstIn->SessionKey, sizeof(pstIn->SessionKey)))

		return -1;


	if(-1 == poNetData->AddString(pstIn->UserId, sizeof(pstIn->UserId)))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->TableId))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->major))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->minor))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->revision))

		return -1;


	if(-1 == poNetData->AddString(pstIn->KEY, sizeof(pstIn->KEY)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_LOGIN_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_LOGIN_REQ *pstOut = (CLI_GS_LOGIN_REQ*)pHost;


	if(-1 == poNetData->DelString(pstOut->SessionKey, sizeof(pstOut->SessionKey)))

		return -1;


	if(-1 == poNetData->DelString(pstOut->UserId, sizeof(pstOut->UserId)))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->TableId))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->major))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->minor))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->revision))

		return -1;


	if(-1 == poNetData->DelString(pstOut->KEY, sizeof(pstOut->KEY)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_LOGIN_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_LOGIN_ACK *pstIn = (GS_CLI_LOGIN_ACK*)pHost;


	if(-1 == poNetData->AddInt(pstIn->ErrCode))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Id))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_LOGIN_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_LOGIN_ACK *pstOut = (GS_CLI_LOGIN_ACK*)pHost;


	if(-1 == poNetData->DelInt(pstOut->ErrCode))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Id))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_PING_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_PING_REQ *pstIn = (CLI_GS_PING_REQ*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Data))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_PING_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_PING_REQ *pstOut = (CLI_GS_PING_REQ*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Data))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_PING_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_PING_ACK *pstIn = (GS_CLI_PING_ACK*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Data))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_PING_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_PING_ACK *pstOut = (GS_CLI_PING_ACK*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Data))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_CREATURE_APPEAR_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_APPEAR_NTF *pstIn = (GS_CLI_CREATURE_APPEAR_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Type))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Career))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Gender))

		return -1;


	if(-1 == poNetData->AddString(pstIn->Name, sizeof(pstIn->Name)))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddString(pstIn->UserId, sizeof(pstIn->UserId)))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->MasterId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->PetTypeId))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->PetStatus))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->MountId))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->MountStatus))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->X))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Y))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Angle))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Radius))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->MovementSpeed))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Health))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Hp))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Mana))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Mp))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Fury))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->FuryMax))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->CampId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->ForceId))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Attackable))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Talkable))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->MonsterClass))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->TableId))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->State))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Level))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->isBuilding))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->OwnerForceId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_CREATURE_APPEAR_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_APPEAR_NTF *pstOut = (GS_CLI_CREATURE_APPEAR_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Type))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Career))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Gender))

		return -1;


	if(-1 == poNetData->DelString(pstOut->Name, sizeof(pstOut->Name)))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelString(pstOut->UserId, sizeof(pstOut->UserId)))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->MasterId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->PetTypeId))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->PetStatus))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->MountId))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->MountStatus))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->X))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Y))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Angle))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Radius))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->MovementSpeed))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Health))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Hp))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Mana))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Mp))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Fury))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->FuryMax))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->CampId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->ForceId))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Attackable))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Talkable))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->MonsterClass))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->TableId))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->State))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Level))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->isBuilding))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->OwnerForceId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_CREATURE_DISAPPEAR_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_DISAPPEAR_NTF *pstIn = (GS_CLI_CREATURE_DISAPPEAR_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_CREATURE_DISAPPEAR_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_DISAPPEAR_NTF *pstOut = (GS_CLI_CREATURE_DISAPPEAR_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_CREATURE_ON_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_ON_NTF *pstIn = (GS_CLI_CREATURE_ON_NTF*)pHost;


	if(-1 == poNetData->AddString(pstIn->Name, sizeof(pstIn->Name)))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_CREATURE_ON_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_ON_NTF *pstOut = (GS_CLI_CREATURE_ON_NTF*)pHost;


	if(-1 == poNetData->DelString(pstOut->Name, sizeof(pstOut->Name)))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_CREATURE_OFF_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_OFF_NTF *pstIn = (GS_CLI_CREATURE_OFF_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_CREATURE_OFF_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_OFF_NTF *pstOut = (GS_CLI_CREATURE_OFF_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_CREATURE_PROPERTIES_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_CREATURE_PROPERTIES_REQ *pstIn = (CLI_GS_CREATURE_PROPERTIES_REQ*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_CREATURE_PROPERTIES_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_CREATURE_PROPERTIES_REQ *pstOut = (CLI_GS_CREATURE_PROPERTIES_REQ*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_CREATURE_PROPERTIES_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_PROPERTIES_ACK *pstIn = (GS_CLI_CREATURE_PROPERTIES_ACK*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddString(pstIn->Name, sizeof(pstIn->Name)))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Exp))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->PreExp))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->NextExp))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Level))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Hp))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Mp))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Strength))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Intelligence))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->PhysicalDamage))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->PhysicalDefence))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->AbilityPower))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->MagicResistance))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Health))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Mana))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->HitValue))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->DodgeValue))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->CriticalStrikeDamage))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->AntiCriticalValue))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->SlayValue))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->ArmorPenetration))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Armor))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->DamageIncreased))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->DamageReduction))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->MovementSpeed))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->AttackRange))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->ManaRegeneration))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->HealthRegeneration))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Tenacity))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Fury))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->AttackSpeed))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->FuryMax))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Bumper))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->VipBumper))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->BumperRate))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Gold))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr1))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr2))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr3))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr4))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr5))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr6))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr7))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr8))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr9))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr10))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr11))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr12))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr13))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr14))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr15))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr16))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr17))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr18))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr19))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Attr20))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->VipLevel))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->CustomizedStatus))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Weapon))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->OwnerForceId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->UnionName, sizeof(pstIn->UnionName)))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->GameTime))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->RMB))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->GM))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->PetTypeId))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->PetStatus))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->MountId))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->MountStatus))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_CREATURE_PROPERTIES_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_CREATURE_PROPERTIES_ACK *pstOut = (GS_CLI_CREATURE_PROPERTIES_ACK*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelString(pstOut->Name, sizeof(pstOut->Name)))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Exp))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->PreExp))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->NextExp))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Level))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Hp))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Mp))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Strength))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Intelligence))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->PhysicalDamage))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->PhysicalDefence))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->AbilityPower))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->MagicResistance))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Health))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Mana))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->HitValue))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->DodgeValue))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->CriticalStrikeDamage))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->AntiCriticalValue))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->SlayValue))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->ArmorPenetration))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Armor))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->DamageIncreased))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->DamageReduction))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->MovementSpeed))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->AttackRange))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->ManaRegeneration))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->HealthRegeneration))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Tenacity))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Fury))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->AttackSpeed))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->FuryMax))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Bumper))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->VipBumper))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->BumperRate))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Gold))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr1))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr2))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr3))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr4))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr5))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr6))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr7))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr8))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr9))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr10))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr11))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr12))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr13))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr14))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr15))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr16))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr17))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr18))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr19))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Attr20))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->VipLevel))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->CustomizedStatus))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Weapon))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->OwnerForceId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->UnionName, sizeof(pstOut->UnionName)))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->GameTime))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->RMB))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->GM))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->PetTypeId))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->PetStatus))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->MountId))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->MountStatus))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_OPERATOR_APPEAR_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_OPERATOR_APPEAR_NTF *pstIn = (GS_CLI_OPERATOR_APPEAR_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Type))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->X))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Y))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Angle))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->SubType))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg1))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg2))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg3))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg4))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg5))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg6))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_OPERATOR_APPEAR_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_OPERATOR_APPEAR_NTF *pstOut = (GS_CLI_OPERATOR_APPEAR_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Type))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->X))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Y))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Angle))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->SubType))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg1))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg2))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg3))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg4))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg5))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg6))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_OPERATOR_DISAPPEAR_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_OPERATOR_DISAPPEAR_NTF *pstIn = (GS_CLI_OPERATOR_DISAPPEAR_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_OPERATOR_DISAPPEAR_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_OPERATOR_DISAPPEAR_NTF *pstOut = (GS_CLI_OPERATOR_DISAPPEAR_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_SIMPLE_MESSAGE_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_SIMPLE_MESSAGE_NTF *pstIn = (GS_CLI_SIMPLE_MESSAGE_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->n))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_SIMPLE_MESSAGE_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_SIMPLE_MESSAGE_NTF *pstOut = (GS_CLI_SIMPLE_MESSAGE_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->n))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_SKILL_PROPERTIES_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_SKILL_PROPERTIES_NTF *pstIn = (GS_CLI_SKILL_PROPERTIES_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Cooldown))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Range))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Active))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_SKILL_PROPERTIES_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_SKILL_PROPERTIES_NTF *pstOut = (GS_CLI_SKILL_PROPERTIES_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Cooldown))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Range))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Active))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_COMMAND_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_COMMAND_REQ *pstIn = (CLI_GS_COMMAND_REQ*)pHost;


	if(-1 == poNetData->AddDword(pstIn->ClientSideId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->Command, sizeof(pstIn->Command)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_COMMAND_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_COMMAND_REQ *pstOut = (CLI_GS_COMMAND_REQ*)pHost;


	if(-1 == poNetData->DelDword(pstOut->ClientSideId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->Command, sizeof(pstOut->Command)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_COMMAND_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_COMMAND_ACK *pstIn = (GS_CLI_COMMAND_ACK*)pHost;


	if(-1 == poNetData->AddDword(pstIn->ClientSideId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->Result, sizeof(pstIn->Result)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_COMMAND_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_COMMAND_ACK *pstOut = (GS_CLI_COMMAND_ACK*)pHost;


	if(-1 == poNetData->DelDword(pstOut->ClientSideId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->Result, sizeof(pstOut->Result)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_QUEST_RESULT_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_QUEST_RESULT_REQ *pstIn = (CLI_GS_QUEST_RESULT_REQ*)pHost;


	if(-1 == poNetData->AddString(pstIn->Result, sizeof(pstIn->Result)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_QUEST_RESULT_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_QUEST_RESULT_REQ *pstOut = (CLI_GS_QUEST_RESULT_REQ*)pHost;


	if(-1 == poNetData->DelString(pstOut->Result, sizeof(pstOut->Result)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_MOVE_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_MOVE_REQ *pstIn = (CLI_GS_MOVE_REQ*)pHost;


	if(-1 == poNetData->AddInt(pstIn->State))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->X))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Y))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Angle))

		return -1;


	if(-1 == poNetData->AddString(pstIn->KEY, sizeof(pstIn->KEY)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_MOVE_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_MOVE_REQ *pstOut = (CLI_GS_MOVE_REQ*)pHost;


	if(-1 == poNetData->DelInt(pstOut->State))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->X))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Y))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Angle))

		return -1;


	if(-1 == poNetData->DelString(pstOut->KEY, sizeof(pstOut->KEY)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_MOVE_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_MOVE_NTF *pstIn = (GS_CLI_MOVE_NTF*)pHost;


	if(-1 == poNetData->AddInt(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->State))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->X))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Y))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Angle))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_MOVE_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_MOVE_NTF *pstOut = (GS_CLI_MOVE_NTF*)pHost;


	if(-1 == poNetData->DelInt(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->State))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->X))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Y))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Angle))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_MOVE_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_MOVE_ACK *pstIn = (GS_CLI_MOVE_ACK*)pHost;


	if(-1 == poNetData->AddFloat(pstIn->X))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Y))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Angle))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_MOVE_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_MOVE_ACK *pstOut = (GS_CLI_MOVE_ACK*)pHost;


	if(-1 == poNetData->DelFloat(pstOut->X))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Y))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Angle))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_FORCE_REPOSITIONED_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_FORCE_REPOSITIONED_NTF *pstIn = (GS_CLI_FORCE_REPOSITIONED_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->X))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Y))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Angle))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Speed))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->ModifierId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_FORCE_REPOSITIONED_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_FORCE_REPOSITIONED_NTF *pstOut = (GS_CLI_FORCE_REPOSITIONED_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->X))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Y))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Angle))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Speed))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->ModifierId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_JUMP_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_JUMP_NTF *pstIn = (GS_CLI_JUMP_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->X))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Y))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Angle))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_JUMP_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_JUMP_NTF *pstOut = (GS_CLI_JUMP_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->X))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Y))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Angle))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_PROPERTYCHANGED_INT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_PROPERTYCHANGED_INT_NTF *pstIn = (GS_CLI_PROPERTYCHANGED_INT_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->PropertyId))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Value))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->ModifierId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg1))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_PROPERTYCHANGED_INT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_PROPERTYCHANGED_INT_NTF *pstOut = (GS_CLI_PROPERTYCHANGED_INT_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->PropertyId))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Value))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->ModifierId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg1))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_PROPERTYCHANGED_UINT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_PROPERTYCHANGED_UINT_NTF *pstIn = (GS_CLI_PROPERTYCHANGED_UINT_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->PropertyId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Value))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->ModifierId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg1))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_PROPERTYCHANGED_UINT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_PROPERTYCHANGED_UINT_NTF *pstOut = (GS_CLI_PROPERTYCHANGED_UINT_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->PropertyId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Value))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->ModifierId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg1))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_PROPERTYCHANGED_FLOAT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_PROPERTYCHANGED_FLOAT_NTF *pstIn = (GS_CLI_PROPERTYCHANGED_FLOAT_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->Id))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->PropertyId))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->Value))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->ModifierId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg1))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_PROPERTYCHANGED_FLOAT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_PROPERTYCHANGED_FLOAT_NTF *pstOut = (GS_CLI_PROPERTYCHANGED_FLOAT_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->Id))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->PropertyId))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->Value))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->ModifierId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg1))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_CASTING_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_CASTING_REQ *pstIn = (CLI_GS_CASTING_REQ*)pHost;


	if(-1 == poNetData->AddByte(pstIn->Type))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillSeq))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->TargetId))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->X))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Y))

		return -1;


	if(-1 == poNetData->AddString(pstIn->KEY, sizeof(pstIn->KEY)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_CASTING_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_CASTING_REQ *pstOut = (CLI_GS_CASTING_REQ*)pHost;


	if(-1 == poNetData->DelByte(pstOut->Type))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillSeq))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->TargetId))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->X))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Y))

		return -1;


	if(-1 == poNetData->DelString(pstOut->KEY, sizeof(pstOut->KEY)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_CASTING_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_CASTING_ACK *pstIn = (GS_CLI_CASTING_ACK*)pHost;


	if(-1 == poNetData->AddDword(pstIn->SkillId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillSeq))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->ErrorId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_CASTING_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_CASTING_ACK *pstOut = (GS_CLI_CASTING_ACK*)pHost;


	if(-1 == poNetData->DelDword(pstOut->SkillId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillSeq))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->ErrorId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_CASTING_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CASTING_NTF *pstIn = (GS_CLI_CASTING_NTF*)pHost;


	if(-1 == poNetData->AddString(pstIn->Json, sizeof(pstIn->Json)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_CASTING_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CASTING_NTF *pstOut = (GS_CLI_CASTING_NTF*)pHost;


	if(-1 == poNetData->DelString(pstOut->Json, sizeof(pstOut->Json)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_CASTED_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CASTED_NTF *pstIn = (GS_CLI_CASTED_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->SkillId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillSeq))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->PlayerId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->TargetId))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->ResultId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_CASTED_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_CASTED_NTF *pstOut = (GS_CLI_CASTED_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->SkillId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillSeq))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->PlayerId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->TargetId))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->ResultId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_SKILL_COOLDOWN_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_SKILL_COOLDOWN_NTF *pstIn = (GS_CLI_SKILL_COOLDOWN_NTF*)pHost;


	if(-1 == poNetData->AddByte(pstIn->Type))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Cooldown))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_SKILL_COOLDOWN_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_SKILL_COOLDOWN_NTF *pstOut = (GS_CLI_SKILL_COOLDOWN_NTF*)pHost;


	if(-1 == poNetData->DelByte(pstOut->Type))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Cooldown))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_PASSIVE_SKILL_ACTIVE(void *pHost, CNetData* poNetData)
{
	GS_CLI_PASSIVE_SKILL_ACTIVE *pstIn = (GS_CLI_PASSIVE_SKILL_ACTIVE*)pHost;


	if(-1 == poNetData->AddByte(pstIn->Type))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->SkillId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_PASSIVE_SKILL_ACTIVE(void *pHost, CNetData* poNetData)
{
	GS_CLI_PASSIVE_SKILL_ACTIVE *pstOut = (GS_CLI_PASSIVE_SKILL_ACTIVE*)pHost;


	if(-1 == poNetData->DelByte(pstOut->Type))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->SkillId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_ADD_BUFF_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_ADD_BUFF_NTF *pstIn = (GS_CLI_ADD_BUFF_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->PlayerId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->BuffId))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->BuffLevel))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->BuffTime))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Count))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->BuffValue))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_ADD_BUFF_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_ADD_BUFF_NTF *pstOut = (GS_CLI_ADD_BUFF_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->PlayerId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->BuffId))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->BuffLevel))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->BuffTime))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Count))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->BuffValue))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_UPDATE_BUFF_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_UPDATE_BUFF_NTF *pstIn = (GS_CLI_UPDATE_BUFF_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->PlayerId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->BuffId))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->BuffLevel))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->BuffTime))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Count))

		return -1;


	if(-1 == poNetData->AddFloat(pstIn->BuffValue))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_UPDATE_BUFF_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_UPDATE_BUFF_NTF *pstOut = (GS_CLI_UPDATE_BUFF_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->PlayerId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->BuffId))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->BuffLevel))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->BuffTime))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Count))

		return -1;


	if(-1 == poNetData->DelFloat(pstOut->BuffValue))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_DEL_BUFF_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_DEL_BUFF_NTF *pstIn = (GS_CLI_DEL_BUFF_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->PlayerId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->BuffId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_DEL_BUFF_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_DEL_BUFF_NTF *pstOut = (GS_CLI_DEL_BUFF_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->PlayerId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->BuffId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_UPDATED_EQUIPMENT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_UPDATED_EQUIPMENT_NTF *pstIn = (GS_CLI_UPDATED_EQUIPMENT_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->OwnerId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->EquipmentId, sizeof(pstIn->EquipmentId)))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Position))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->ItemType))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Count))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->TemplateId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->ExtraJson, sizeof(pstIn->ExtraJson)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_UPDATED_EQUIPMENT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_UPDATED_EQUIPMENT_NTF *pstOut = (GS_CLI_UPDATED_EQUIPMENT_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->OwnerId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->EquipmentId, sizeof(pstOut->EquipmentId)))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Position))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->ItemType))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Count))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->TemplateId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->ExtraJson, sizeof(pstOut->ExtraJson)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_LIST_EQUIPMENT_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_LIST_EQUIPMENT_REQ *pstIn = (CLI_GS_LIST_EQUIPMENT_REQ*)pHost;


	if(-1 == poNetData->AddDword(pstIn->OwnerId))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Bag))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Equipment))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_LIST_EQUIPMENT_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_LIST_EQUIPMENT_REQ *pstOut = (CLI_GS_LIST_EQUIPMENT_REQ*)pHost;


	if(-1 == poNetData->DelDword(pstOut->OwnerId))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Bag))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Equipment))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_LIST_EQUIPMENT_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_LIST_EQUIPMENT_ACK *pstIn = (GS_CLI_LIST_EQUIPMENT_ACK*)pHost;


	if(-1 == poNetData->AddDword(pstIn->OwnerId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->EquipmentId, sizeof(pstIn->EquipmentId)))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Position))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->ItemType))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Count))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->TemplateId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->ExtraJson, sizeof(pstIn->ExtraJson)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_LIST_EQUIPMENT_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_LIST_EQUIPMENT_ACK *pstOut = (GS_CLI_LIST_EQUIPMENT_ACK*)pHost;


	if(-1 == poNetData->DelDword(pstOut->OwnerId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->EquipmentId, sizeof(pstOut->EquipmentId)))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Position))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->ItemType))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Count))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->TemplateId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->ExtraJson, sizeof(pstOut->ExtraJson)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_LIST_EQUIPMENT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_LIST_EQUIPMENT_NTF *pstIn = (GS_CLI_LIST_EQUIPMENT_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->OwnerId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->EquipmentId, sizeof(pstIn->EquipmentId)))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Position))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->ItemType))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Count))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->TemplateId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->ExtraJson, sizeof(pstIn->ExtraJson)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_LIST_EQUIPMENT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_LIST_EQUIPMENT_NTF *pstOut = (GS_CLI_LIST_EQUIPMENT_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->OwnerId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->EquipmentId, sizeof(pstOut->EquipmentId)))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Position))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->ItemType))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Count))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->TemplateId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->ExtraJson, sizeof(pstOut->ExtraJson)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_DROP_EQUIPMENT_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_DROP_EQUIPMENT_REQ *pstIn = (CLI_GS_DROP_EQUIPMENT_REQ*)pHost;


	if(-1 == poNetData->AddByte(pstIn->Position))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_DROP_EQUIPMENT_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_DROP_EQUIPMENT_REQ *pstOut = (CLI_GS_DROP_EQUIPMENT_REQ*)pHost;


	if(-1 == poNetData->DelByte(pstOut->Position))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_DROP_EQUIPMENT_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_DROP_EQUIPMENT_ACK *pstIn = (GS_CLI_DROP_EQUIPMENT_ACK*)pHost;


	if(-1 == poNetData->AddByte(pstIn->Position))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->ErrorId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_DROP_EQUIPMENT_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_DROP_EQUIPMENT_ACK *pstOut = (GS_CLI_DROP_EQUIPMENT_ACK*)pHost;


	if(-1 == poNetData->DelByte(pstOut->Position))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->ErrorId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_USE_CONSUME_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_USE_CONSUME_REQ *pstIn = (CLI_GS_USE_CONSUME_REQ*)pHost;


	if(-1 == poNetData->AddByte(pstIn->Position))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->UseSeq))

		return -1;


	if(-1 == poNetData->AddString(pstIn->KEY, sizeof(pstIn->KEY)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_USE_CONSUME_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_USE_CONSUME_REQ *pstOut = (CLI_GS_USE_CONSUME_REQ*)pHost;


	if(-1 == poNetData->DelByte(pstOut->Position))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->UseSeq))

		return -1;


	if(-1 == poNetData->DelString(pstOut->KEY, sizeof(pstOut->KEY)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_USE_CONSUME_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_USE_CONSUME_ACK *pstIn = (GS_CLI_USE_CONSUME_ACK*)pHost;


	if(-1 == poNetData->AddByte(pstIn->Position))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->UseSeq))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->ErrorId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_USE_CONSUME_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_USE_CONSUME_ACK *pstOut = (GS_CLI_USE_CONSUME_ACK*)pHost;


	if(-1 == poNetData->DelByte(pstOut->Position))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->UseSeq))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->ErrorId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_MOVE_EQUIPMENT_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_MOVE_EQUIPMENT_REQ *pstIn = (CLI_GS_MOVE_EQUIPMENT_REQ*)pHost;


	if(-1 == poNetData->AddDword(pstIn->SourceOwnerId))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->PositionFrom))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->PositionTo))

		return -1;


	if(-1 == poNetData->AddString(pstIn->KEY, sizeof(pstIn->KEY)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_MOVE_EQUIPMENT_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_MOVE_EQUIPMENT_REQ *pstOut = (CLI_GS_MOVE_EQUIPMENT_REQ*)pHost;


	if(-1 == poNetData->DelDword(pstOut->SourceOwnerId))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->PositionFrom))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->PositionTo))

		return -1;


	if(-1 == poNetData->DelString(pstOut->KEY, sizeof(pstOut->KEY)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_MOVE_EQUIPMENT_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_MOVE_EQUIPMENT_ACK *pstIn = (GS_CLI_MOVE_EQUIPMENT_ACK*)pHost;


	if(-1 == poNetData->AddDword(pstIn->ErrorId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->EquipmentId, sizeof(pstIn->EquipmentId)))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Position))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->ItemType))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Count))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->TemplateId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->ExtraJson, sizeof(pstIn->ExtraJson)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_MOVE_EQUIPMENT_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_MOVE_EQUIPMENT_ACK *pstOut = (GS_CLI_MOVE_EQUIPMENT_ACK*)pHost;


	if(-1 == poNetData->DelDword(pstOut->ErrorId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->EquipmentId, sizeof(pstOut->EquipmentId)))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Position))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->ItemType))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Count))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->TemplateId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->ExtraJson, sizeof(pstOut->ExtraJson)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_MOVE_EQUIPMENT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_MOVE_EQUIPMENT_NTF *pstIn = (GS_CLI_MOVE_EQUIPMENT_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->OwnerId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->EquipmentId, sizeof(pstIn->EquipmentId)))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->Position))

		return -1;


	if(-1 == poNetData->AddByte(pstIn->ItemType))

		return -1;


	if(-1 == poNetData->AddInt(pstIn->Count))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->TemplateId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->ExtraJson, sizeof(pstIn->ExtraJson)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_MOVE_EQUIPMENT_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_MOVE_EQUIPMENT_NTF *pstOut = (GS_CLI_MOVE_EQUIPMENT_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->OwnerId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->EquipmentId, sizeof(pstOut->EquipmentId)))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->Position))

		return -1;


	if(-1 == poNetData->DelByte(pstOut->ItemType))

		return -1;


	if(-1 == poNetData->DelInt(pstOut->Count))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->TemplateId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->ExtraJson, sizeof(pstOut->ExtraJson)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_QUERY_OPERATOR_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_QUERY_OPERATOR_REQ *pstIn = (CLI_GS_QUERY_OPERATOR_REQ*)pHost;


	if(-1 == poNetData->AddDword(pstIn->TargetId))

		return -1;


	if(-1 == poNetData->AddDword(pstIn->Arg1))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_QUERY_OPERATOR_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_QUERY_OPERATOR_REQ *pstOut = (CLI_GS_QUERY_OPERATOR_REQ*)pHost;


	if(-1 == poNetData->DelDword(pstOut->TargetId))

		return -1;


	if(-1 == poNetData->DelDword(pstOut->Arg1))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_CHANGE_TABLE_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_CHANGE_TABLE_REQ *pstIn = (CLI_GS_CHANGE_TABLE_REQ*)pHost;


	if(-1 == poNetData->AddInt(pstIn->TableId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_CHANGE_TABLE_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_CHANGE_TABLE_REQ *pstOut = (CLI_GS_CHANGE_TABLE_REQ*)pHost;


	if(-1 == poNetData->DelInt(pstOut->TableId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_CHANGE_TABLE_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_CHANGE_TABLE_ACK *pstIn = (GS_CLI_CHANGE_TABLE_ACK*)pHost;


	if(-1 == poNetData->AddInt(pstIn->TableId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_CHANGE_TABLE_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_CHANGE_TABLE_ACK *pstOut = (GS_CLI_CHANGE_TABLE_ACK*)pHost;


	if(-1 == poNetData->DelInt(pstOut->TableId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_REMOVE_QTE_BUFF_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_REMOVE_QTE_BUFF_REQ *pstIn = (CLI_GS_REMOVE_QTE_BUFF_REQ*)pHost;


	if(-1 == poNetData->AddDword(pstIn->BuffId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_REMOVE_QTE_BUFF_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_REMOVE_QTE_BUFF_REQ *pstOut = (CLI_GS_REMOVE_QTE_BUFF_REQ*)pHost;


	if(-1 == poNetData->DelDword(pstOut->BuffId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_REMOVE_QTE_BUFF_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_REMOVE_QTE_BUFF_ACK *pstIn = (GS_CLI_REMOVE_QTE_BUFF_ACK*)pHost;


	if(-1 == poNetData->AddDword(pstIn->BuffId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_REMOVE_QTE_BUFF_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_REMOVE_QTE_BUFF_ACK *pstOut = (GS_CLI_REMOVE_QTE_BUFF_ACK*)pHost;


	if(-1 == poNetData->DelDword(pstOut->BuffId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_LOCK_PLAYER_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_LOCK_PLAYER_REQ *pstIn = (CLI_GS_LOCK_PLAYER_REQ*)pHost;


	if(-1 == poNetData->AddByte(pstIn->Lock))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_LOCK_PLAYER_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_LOCK_PLAYER_REQ *pstOut = (CLI_GS_LOCK_PLAYER_REQ*)pHost;


	if(-1 == poNetData->DelByte(pstOut->Lock))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_WINNER_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_WINNER_NTF *pstIn = (GS_CLI_WINNER_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->ForceId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_WINNER_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_WINNER_NTF *pstOut = (GS_CLI_WINNER_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->ForceId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_SPEAK_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_SPEAK_NTF *pstIn = (GS_CLI_SPEAK_NTF*)pHost;


	if(-1 == poNetData->AddDword(pstIn->SpeakerId))

		return -1;


	if(-1 == poNetData->AddString(pstIn->Content, sizeof(pstIn->Content)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_SPEAK_NTF(void *pHost, CNetData* poNetData)
{
	GS_CLI_SPEAK_NTF *pstOut = (GS_CLI_SPEAK_NTF*)pHost;


	if(-1 == poNetData->DelDword(pstOut->SpeakerId))

		return -1;


	if(-1 == poNetData->DelString(pstOut->Content, sizeof(pstOut->Content)))

		return -1;

	return sizeof(*pstOut);
}
int EncodeCLI_GS_TALK_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_TALK_REQ *pstIn = (CLI_GS_TALK_REQ*)pHost;


	if(-1 == poNetData->AddDword(pstIn->TargetId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeCLI_GS_TALK_REQ(void *pHost, CNetData* poNetData)
{
	CLI_GS_TALK_REQ *pstOut = (CLI_GS_TALK_REQ*)pHost;


	if(-1 == poNetData->DelDword(pstOut->TargetId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_TALK_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_TALK_ACK *pstIn = (GS_CLI_TALK_ACK*)pHost;


	if(-1 == poNetData->AddInt(pstIn->ResultId))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_TALK_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_TALK_ACK *pstOut = (GS_CLI_TALK_ACK*)pHost;


	if(-1 == poNetData->DelInt(pstOut->ResultId))

		return -1;

	return sizeof(*pstOut);
}
int EncodeGS_CLI_NOTIFY_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_NOTIFY_ACK *pstIn = (GS_CLI_NOTIFY_ACK*)pHost;


	if(-1 == poNetData->AddString(pstIn->Json, sizeof(pstIn->Json)))

		return -1;

	return poNetData->GetDataLen();
}
int DecodeGS_CLI_NOTIFY_ACK(void *pHost, CNetData* poNetData)
{
	GS_CLI_NOTIFY_ACK *pstOut = (GS_CLI_NOTIFY_ACK*)pHost;


	if(-1 == poNetData->DelString(pstOut->Json, sizeof(pstOut->Json)))

		return -1;

	return sizeof(*pstOut);
}


