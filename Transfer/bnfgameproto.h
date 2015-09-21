/********************************************************************
**       This head file is generated by program,                   **
**            Please do not edit it directly.                    **
********************************************************************/
/*       Version Number: 1          */
#ifndef BNFGAMEPROTO_H_INCLUDE_VERSION_1
#define BNFGAMEPROTO_H_INCLUDE_VERSION_1
#include "netdata.h"
/*Defines Begin*/

#define VERSION_MAJOR 0
#define VERSION_MINOR 8
#define VERSION_REVISION 6
#define SESSION_KEY_LEN 33
#define NAME_LEN 128
#define PROTOCOL_KEY_LEN 33
#define EQUIPMENT_ID_LEN 33
#define EQUIPMENT_NAME_LEN 256
#define EQUIPMENT_RES_LEN 33
#define EQUIPMENT_GRADE_LEN 20
#define EQUIPMENT_TYPE_LEN 50
#define EQUIPMENT_PRICETYPE_LEN 20
#define MAX_COMMAND_LEN 1000
#define MAX_QUEST_RESULT_LEN 512
#define MEDIUM_TEXT_LEN 1024
#define MAX_TEXT_LEN 4096
#define BNFGAMEPROTO_MSGID_RANGE 3001
#define BNFGAMEPROTO_MSGID_OFFSET 2000
/*Defines End*/
enum EN_BnFGameProto_MessageID
{

    MSG_LOGIN_REQ = 2000,

    MSG_LOGIN_ACK = 2001,

    MSG_PING_REQ = 2003,

    MSG_PING_ACK = 2004,

    MSG_MOVE_REQ = 2010,

    MSG_MOVE_NTF = 2011,

    MSG_MOVE_ACK = 2012,

    MSG_PROPERTYCHANGED_INT_NTF = 2013,

    MSG_PROPERTYCHANGED_UINT_NTF = 2014,

    MSG_PROPERTYCHANGED_FLOAT_NTF = 2015,

    MSG_FORCE_REPOSITIONED_NTF = 2016,

    MSG_CREATURE_PROPERTIES_REQ = 2017,

    MSG_CREATURE_PROPERTIES_ACK = 2018,

    MSG_JUMP_NTF = 2019,

    MSG_SKILL_PROPERTIES_NTF = 2020,

    MSG_CASTING_REQ = 2022,

    MSG_CASTING_NTF = 2023,

    MSG_CASTED_NTF = 2024,

    MSG_CASTING_ACK = 2025,

    MSG_SKILL_COOLDOWN_NTF = 2026,

    MSG_PASSIVE_SKILL_ACTIVE = 2027,

    MSG_CREATURE_APPEAR_NTF = 2030,

    MSG_CREATURE_DISAPPEAR_NTF = 2031,

    MSG_CREATURE_ON_NTF = 2032,

    MSG_CREATURE_OFF_NTF = 2033,

    MSG_OPERATOR_APPEAR_NTF = 2034,

    MSG_OPERATOR_DISAPPEAR_NTF = 2035,

    MSG_ADD_BUFF_NTF = 2040,

    MSG_UPDATE_BUFF_NTF = 2041,

    MSG_DEL_BUFF_NTF = 2042,

    MSG_MOVE_EQUIPMENT_REQ = 2050,

    MSG_MOVE_EQUIPMENT_ACK = 2060,

    MSG_MOVE_EQUIPMENT_NTF = 2070,

    MSG_DROP_EQUIPMENT_REQ = 2080,

    MSG_DROP_EQUIPMENT_ACK = 2081,

    MSG_USE_CONSUME_REQ = 2082,

    MSG_USE_CONSUME_ACK = 2083,

    MSG_UPDATED_EQUIPMENT_NTF = 2110,

    MSG_LIST_EQUIPMENT_REQ = 2120,

    MSG_LIST_EQUIPMENT_ACK = 2130,

    MSG_LIST_EQUIPMENT_NTF = 2140,

    MSG_SIMPLE_MESSAGE_NTF = 2150,

    MSG_COMMAND_REQ = 3101,

    MSG_COMMAND_ACK = 3102,

    MSG_QUEST_RESULT_REQ = 3103,

    MSG_QUERY_OPERATOR_REQ = 4120,

    MSG_CHANGE_TABLE_REQ = 4130,

    MSG_CHANGE_TABLE_ACK = 4131,

    MSG_REMOVE_QTE_BUFF_REQ = 4132,

    MSG_REMOVE_QTE_BUFF_ACK = 4133,

    MSG_LOCK_PLAYER_REQ = 4200,

    MSG_WINNER_NTF = 4300,

    MSG_SPEAK_NTF = 4401,

    MSG_TALK_REQ = 4402,

    MSG_TALK_ACK = 4403,

    MSG_NOTIFY_JSON = 5000,

    MSG_JUMP_SCENE_REQ = 4466,

    MSG_JUMP_SCENE_ACK = 4467,

};
/*Define Structs and Unions        Begin*/

typedef struct tagCLI_GS_JUMP_SCENE_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_JUMP_SCENE_REQ; }

    char	Host[SESSION_KEY_LEN];

    char	UserId[SESSION_KEY_LEN];

    int	Port;

} CLI_GS_JUMP_SCENE_REQ;
typedef struct tagGS_CLI_JUMP_SCENE_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_JUMP_SCENE_ACK; }

    int	ErrCode;

    int	Id;

} GS_CLI_JUMP_SCENE_ACK;
typedef struct tagCLI_GS_LOGIN_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_LOGIN_REQ; }

    char	SessionKey[SESSION_KEY_LEN];

    char	UserId[SESSION_KEY_LEN];

    int	TableId;

    int	major;

    int	minor;

    int	revision;

    char	KEY[PROTOCOL_KEY_LEN];

} CLI_GS_LOGIN_REQ;
typedef struct tagGS_CLI_LOGIN_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_LOGIN_ACK; }

    int	ErrCode;

    int	Id;

} GS_CLI_LOGIN_ACK;
typedef struct tagCLI_GS_PING_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_PING_REQ; }

    unsigned int	Data;

} CLI_GS_PING_REQ;
typedef struct tagGS_CLI_PING_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_PING_ACK; }

    unsigned int	Data;

} GS_CLI_PING_ACK;
typedef struct tagGS_CLI_CREATURE_APPEAR_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CREATURE_APPEAR_NTF; }

    unsigned int	Type;

    unsigned int	Career;

    unsigned char	Gender;

    char	Name[NAME_LEN];

    unsigned int	Id;

    char	UserId[SESSION_KEY_LEN];

    unsigned int	MasterId;

    unsigned int	PetTypeId;

    unsigned char	PetStatus;

    unsigned int	MountId;

    unsigned char	MountStatus;

    float	X;

    float	Y;

    float	Angle;

    float	Radius;

    float	MovementSpeed;

    float	Health;

    float	Hp;

    float	Mana;

    float	Mp;

    float	Fury;

    float	FuryMax;

    unsigned char	CampId;

    unsigned int	ForceId;

    unsigned char	Attackable;

    unsigned char	Talkable;

    int	MonsterClass;

    int	TableId;

    int	State;

    unsigned int	Level;

    unsigned char	isBuilding;

    unsigned int	OwnerForceId;

} GS_CLI_CREATURE_APPEAR_NTF;
typedef struct tagGS_CLI_CREATURE_DISAPPEAR_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CREATURE_DISAPPEAR_NTF; }

    unsigned int	Id;

} GS_CLI_CREATURE_DISAPPEAR_NTF;
typedef struct tagGS_CLI_CREATURE_ON_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CREATURE_ON_NTF; }

    char	Name[NAME_LEN];

    unsigned int	Id;

} GS_CLI_CREATURE_ON_NTF;
typedef struct tagGS_CLI_CREATURE_OFF_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CREATURE_OFF_NTF; }

    unsigned int	Id;

} GS_CLI_CREATURE_OFF_NTF;
typedef struct tagCLI_GS_CREATURE_PROPERTIES_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CREATURE_PROPERTIES_REQ; }

    unsigned int	Id;

} CLI_GS_CREATURE_PROPERTIES_REQ;
typedef struct tagGS_CLI_CREATURE_PROPERTIES_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CREATURE_PROPERTIES_ACK; }

    unsigned int	Id;

    char	Name[NAME_LEN];

    int	Exp;

    int	PreExp;

    int	NextExp;

    int	Level;

    float	Hp;

    float	Mp;

    float	Strength;

    float	Intelligence;

    float	PhysicalDamage;

    float	PhysicalDefence;

    float	AbilityPower;

    float	MagicResistance;

    float	Health;

    float	Mana;

    float	HitValue;

    float	DodgeValue;

    float	CriticalStrikeDamage;

    float	AntiCriticalValue;

    float	SlayValue;

    float	ArmorPenetration;

    float	Armor;

    float	DamageIncreased;

    float	DamageReduction;

    float	MovementSpeed;

    float	AttackRange;

    float	ManaRegeneration;

    float	HealthRegeneration;

    float	Tenacity;

    float	Fury;

    float	AttackSpeed;

    float	FuryMax;

    unsigned int	Bumper;

    unsigned int	VipBumper;

    float	BumperRate;

    unsigned int	Gold;

    unsigned int	Attr1;

    unsigned int	Attr2;

    unsigned int	Attr3;

    unsigned int	Attr4;

    unsigned int	Attr5;

    unsigned int	Attr6;

    unsigned int	Attr7;

    unsigned int	Attr8;

    unsigned int	Attr9;

    unsigned int	Attr10;

    unsigned int	Attr11;

    unsigned int	Attr12;

    unsigned int	Attr13;

    unsigned int	Attr14;

    unsigned int	Attr15;

    unsigned int	Attr16;

    unsigned int	Attr17;

    unsigned int	Attr18;

    unsigned int	Attr19;

    unsigned int	Attr20;

    unsigned int	VipLevel;

    unsigned int	CustomizedStatus;

    unsigned int	Weapon;

    unsigned int	OwnerForceId;

    char	UnionName[NAME_LEN];

    unsigned int	GameTime;

    unsigned int	RMB;

    unsigned int	GM;

    unsigned int	PetTypeId;

    unsigned char	PetStatus;

    unsigned int	MountId;

    unsigned char	MountStatus;

} GS_CLI_CREATURE_PROPERTIES_ACK;
typedef struct tagGS_CLI_OPERATOR_APPEAR_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_OPERATOR_APPEAR_NTF; }

    unsigned int	Id;

    int	Type;

    float	X;

    float	Y;

    float	Angle;

    int	SubType;

    unsigned int	Arg1;

    unsigned int	Arg2;

    unsigned int	Arg3;

    unsigned int	Arg4;

    unsigned int	Arg5;

    unsigned int	Arg6;

} GS_CLI_OPERATOR_APPEAR_NTF;
typedef struct tagGS_CLI_OPERATOR_DISAPPEAR_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_OPERATOR_DISAPPEAR_NTF; }

    unsigned int	Id;

} GS_CLI_OPERATOR_DISAPPEAR_NTF;
typedef struct tagGS_CLI_SIMPLE_MESSAGE_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_SIMPLE_MESSAGE_NTF; }

    unsigned int	n;

} GS_CLI_SIMPLE_MESSAGE_NTF;
typedef struct tagGS_CLI_SKILL_PROPERTIES_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_SKILL_PROPERTIES_NTF; }

    unsigned int	Id;

    float	Cooldown;

    float	Range;

    unsigned char	Active;

} GS_CLI_SKILL_PROPERTIES_NTF;
typedef struct tagCLI_GS_COMMAND_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_COMMAND_REQ; }

    unsigned int	ClientSideId;

    char	Command[MAX_COMMAND_LEN];

} CLI_GS_COMMAND_REQ;
typedef struct tagGS_CLI_COMMAND_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_COMMAND_ACK; }

    unsigned int	ClientSideId;

    char	Result[MAX_COMMAND_LEN];

} GS_CLI_COMMAND_ACK;
typedef struct tagCLI_GS_QUEST_RESULT_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_QUEST_RESULT_REQ; }

    char	Result[MAX_QUEST_RESULT_LEN];

} CLI_GS_QUEST_RESULT_REQ;
typedef struct tagCLI_GS_MOVE_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_MOVE_REQ; }

    int	State;

    int	X;

    int	Y;

    float	Angle;

    char	KEY[PROTOCOL_KEY_LEN];

} CLI_GS_MOVE_REQ;
typedef struct tagGS_CLI_MOVE_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_MOVE_NTF; }

    int	Id;

    int	State;

    float	X;

    float	Y;

    float	Angle;

} GS_CLI_MOVE_NTF;
typedef struct tagGS_CLI_MOVE_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_MOVE_ACK; }

    float	X;

    float	Y;

    float	Angle;

} GS_CLI_MOVE_ACK;
typedef struct tagGS_CLI_FORCE_REPOSITIONED_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_FORCE_REPOSITIONED_NTF; }

    unsigned int	Id;

    float	X;

    float	Y;

    float	Angle;

    float	Speed;

    unsigned int	SkillId;

    unsigned int	ModifierId;

} GS_CLI_FORCE_REPOSITIONED_NTF;
typedef struct tagGS_CLI_JUMP_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_JUMP_NTF; }

    unsigned int	Id;

    float	X;

    float	Y;

    float	Angle;

} GS_CLI_JUMP_NTF;
typedef struct tagGS_CLI_PROPERTYCHANGED_INT_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_PROPERTYCHANGED_INT_NTF; }

    unsigned int	Id;

    int	PropertyId;

    int	Value;

    unsigned int	SkillId;

    unsigned int	ModifierId;

    unsigned int	Arg1;

} GS_CLI_PROPERTYCHANGED_INT_NTF;
typedef struct tagGS_CLI_PROPERTYCHANGED_UINT_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_PROPERTYCHANGED_UINT_NTF; }

    unsigned int	Id;

    int	PropertyId;

    unsigned int	Value;

    unsigned int	SkillId;

    unsigned int	ModifierId;

    unsigned int	Arg1;

} GS_CLI_PROPERTYCHANGED_UINT_NTF;
typedef struct tagGS_CLI_PROPERTYCHANGED_FLOAT_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_PROPERTYCHANGED_FLOAT_NTF; }

    unsigned int	Id;

    int	PropertyId;

    float	Value;

    unsigned int	SkillId;

    unsigned int	ModifierId;

    unsigned int	Arg1;

} GS_CLI_PROPERTYCHANGED_FLOAT_NTF;
typedef struct tagCLI_GS_CASTING_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CASTING_REQ; }

    unsigned char	Type;

    unsigned int	SkillId;

    unsigned int	SkillSeq;

    unsigned int	TargetId;

    int	X;

    int	Y;

    char	KEY[PROTOCOL_KEY_LEN];

} CLI_GS_CASTING_REQ;
typedef struct tagGS_CLI_CASTING_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CASTING_ACK; }

    unsigned int	SkillId;

    unsigned int	SkillSeq;

    unsigned int	ErrorId;

} GS_CLI_CASTING_ACK;
typedef struct tagGS_CLI_CASTING_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CASTING_NTF; }

    char	Json[MAX_TEXT_LEN];

} GS_CLI_CASTING_NTF;
typedef struct tagGS_CLI_CASTED_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CASTED_NTF; }

    unsigned int	SkillId;

    unsigned int	SkillSeq;

    unsigned int	PlayerId;

    unsigned int	TargetId;

    unsigned char	ResultId;

} GS_CLI_CASTED_NTF;
typedef struct tagGS_CLI_SKILL_COOLDOWN_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_SKILL_COOLDOWN_NTF; }

    unsigned char	Type;

    unsigned int	SkillId;

    unsigned int	Cooldown;

} GS_CLI_SKILL_COOLDOWN_NTF;
typedef struct tagGS_CLI_PASSIVE_SKILL_ACTIVE {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_PASSIVE_SKILL_ACTIVE; }

    unsigned char	Type;

    unsigned int	SkillId;

} GS_CLI_PASSIVE_SKILL_ACTIVE;
typedef struct tagGS_CLI_ADD_BUFF_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_ADD_BUFF_NTF; }

    unsigned int	PlayerId;

    unsigned int	BuffId;

    int	BuffLevel;

    unsigned int	BuffTime;

    int	Count;

    float	BuffValue;

} GS_CLI_ADD_BUFF_NTF;
typedef struct tagGS_CLI_UPDATE_BUFF_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_UPDATE_BUFF_NTF; }

    unsigned int	PlayerId;

    unsigned int	BuffId;

    int	BuffLevel;

    unsigned int	BuffTime;

    int	Count;

    float	BuffValue;

} GS_CLI_UPDATE_BUFF_NTF;
typedef struct tagGS_CLI_DEL_BUFF_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_DEL_BUFF_NTF; }

    unsigned int	PlayerId;

    unsigned int	BuffId;

} GS_CLI_DEL_BUFF_NTF;
typedef struct tagGS_CLI_UPDATED_EQUIPMENT_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_UPDATED_EQUIPMENT_NTF; }

    unsigned int	OwnerId;

    char	EquipmentId[EQUIPMENT_ID_LEN];

    unsigned char	Position;

    unsigned char	ItemType;

    int	Count;

    unsigned int	TemplateId;

    char	ExtraJson[MEDIUM_TEXT_LEN];

} GS_CLI_UPDATED_EQUIPMENT_NTF;
typedef struct tagCLI_GS_LIST_EQUIPMENT_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_LIST_EQUIPMENT_REQ; }

    unsigned int	OwnerId;

    unsigned char	Bag;

    unsigned char	Equipment;

} CLI_GS_LIST_EQUIPMENT_REQ;
typedef struct tagGS_CLI_LIST_EQUIPMENT_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_LIST_EQUIPMENT_ACK; }

    unsigned int	OwnerId;

    char	EquipmentId[EQUIPMENT_ID_LEN];

    unsigned char	Position;

    unsigned char	ItemType;

    int	Count;

    unsigned int	TemplateId;

    char	ExtraJson[MEDIUM_TEXT_LEN];

} GS_CLI_LIST_EQUIPMENT_ACK;
typedef struct tagGS_CLI_LIST_EQUIPMENT_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_LIST_EQUIPMENT_NTF; }

    unsigned int	OwnerId;

    char	EquipmentId[EQUIPMENT_ID_LEN];

    unsigned char	Position;

    unsigned char	ItemType;

    int	Count;

    unsigned int	TemplateId;

    char	ExtraJson[MEDIUM_TEXT_LEN];

} GS_CLI_LIST_EQUIPMENT_NTF;
typedef struct tagCLI_GS_DROP_EQUIPMENT_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_DROP_EQUIPMENT_REQ; }

    unsigned char	Position;

} CLI_GS_DROP_EQUIPMENT_REQ;
typedef struct tagGS_CLI_DROP_EQUIPMENT_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_DROP_EQUIPMENT_ACK; }

    unsigned char	Position;

    unsigned int	ErrorId;

} GS_CLI_DROP_EQUIPMENT_ACK;
typedef struct tagCLI_GS_USE_CONSUME_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_USE_CONSUME_REQ; }

    unsigned char	Position;

    int	UseSeq;

    char	KEY[PROTOCOL_KEY_LEN];

} CLI_GS_USE_CONSUME_REQ;
typedef struct tagGS_CLI_USE_CONSUME_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_USE_CONSUME_ACK; }

    unsigned char	Position;

    int	UseSeq;

    unsigned int	ErrorId;

} GS_CLI_USE_CONSUME_ACK;
typedef struct tagCLI_GS_MOVE_EQUIPMENT_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_MOVE_EQUIPMENT_REQ; }

    unsigned int	SourceOwnerId;

    unsigned char	PositionFrom;

    unsigned char	PositionTo;

    char	KEY[PROTOCOL_KEY_LEN];

} CLI_GS_MOVE_EQUIPMENT_REQ;
typedef struct tagGS_CLI_MOVE_EQUIPMENT_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_MOVE_EQUIPMENT_ACK; }

    unsigned int	ErrorId;

    char	EquipmentId[EQUIPMENT_ID_LEN];

    unsigned char	Position;

    unsigned char	ItemType;

    int	Count;

    unsigned int	TemplateId;

    char	ExtraJson[MEDIUM_TEXT_LEN];

} GS_CLI_MOVE_EQUIPMENT_ACK;
typedef struct tagGS_CLI_MOVE_EQUIPMENT_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_MOVE_EQUIPMENT_NTF; }

    unsigned int	OwnerId;

    char	EquipmentId[EQUIPMENT_ID_LEN];

    unsigned char	Position;

    unsigned char	ItemType;

    int	Count;

    unsigned int	TemplateId;

    char	ExtraJson[MEDIUM_TEXT_LEN];

} GS_CLI_MOVE_EQUIPMENT_NTF;
typedef struct tagCLI_GS_QUERY_OPERATOR_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_QUERY_OPERATOR_REQ; }

    unsigned int	TargetId;

    unsigned int	Arg1;

} CLI_GS_QUERY_OPERATOR_REQ;
typedef struct tagCLI_GS_CHANGE_TABLE_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CHANGE_TABLE_REQ; }

    int	TableId;

} CLI_GS_CHANGE_TABLE_REQ;
typedef struct tagGS_CLI_CHANGE_TABLE_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_CHANGE_TABLE_ACK; }

    int	TableId;

} GS_CLI_CHANGE_TABLE_ACK;
typedef struct tagCLI_GS_REMOVE_QTE_BUFF_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_REMOVE_QTE_BUFF_REQ; }

    unsigned int	BuffId;

} CLI_GS_REMOVE_QTE_BUFF_REQ;
typedef struct tagGS_CLI_REMOVE_QTE_BUFF_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_REMOVE_QTE_BUFF_ACK; }

    unsigned int	BuffId;

} GS_CLI_REMOVE_QTE_BUFF_ACK;
typedef struct tagCLI_GS_LOCK_PLAYER_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_LOCK_PLAYER_REQ; }

    unsigned char	Lock;

} CLI_GS_LOCK_PLAYER_REQ;
typedef struct tagGS_CLI_WINNER_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_WINNER_NTF; }

    unsigned int	ForceId;

} GS_CLI_WINNER_NTF;
typedef struct tagGS_CLI_SPEAK_NTF {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_SPEAK_NTF; }

    unsigned int	SpeakerId;

    char	Content[MAX_TEXT_LEN];

} GS_CLI_SPEAK_NTF;
typedef struct tagCLI_GS_TALK_REQ {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_TALK_REQ; }

    unsigned int	TargetId;

} CLI_GS_TALK_REQ;
typedef struct tagGS_CLI_TALK_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_TALK_ACK; }

    int	ResultId;

} GS_CLI_TALK_ACK;
typedef struct tagGS_CLI_NOTIFY_ACK {
    static enum EN_BnFGameProto_MessageID MessageId() { return MSG_NOTIFY_JSON; }

    char	Json[MAX_TEXT_LEN];

} GS_CLI_NOTIFY_ACK;

/*Define Structs and Unions        End*/
/*Encode and Decode Function Declare Begin*/

int EncodeCLI_GS_JUMP_SCENE_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_JUMP_SCENE_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_JUMP_SCENE_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_JUMP_SCENE_ACK(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_LOGIN_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_LOGIN_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_LOGIN_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_LOGIN_ACK(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_PING_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_PING_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_PING_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_PING_ACK(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_CREATURE_APPEAR_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_CREATURE_APPEAR_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_CREATURE_DISAPPEAR_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_CREATURE_DISAPPEAR_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_CREATURE_ON_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_CREATURE_ON_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_CREATURE_OFF_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_CREATURE_OFF_NTF(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_CREATURE_PROPERTIES_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_CREATURE_PROPERTIES_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_CREATURE_PROPERTIES_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_CREATURE_PROPERTIES_ACK(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_OPERATOR_APPEAR_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_OPERATOR_APPEAR_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_OPERATOR_DISAPPEAR_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_OPERATOR_DISAPPEAR_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_SIMPLE_MESSAGE_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_SIMPLE_MESSAGE_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_SKILL_PROPERTIES_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_SKILL_PROPERTIES_NTF(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_COMMAND_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_COMMAND_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_COMMAND_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_COMMAND_ACK(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_QUEST_RESULT_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_QUEST_RESULT_REQ(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_MOVE_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_MOVE_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_MOVE_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_MOVE_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_MOVE_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_MOVE_ACK(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_FORCE_REPOSITIONED_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_FORCE_REPOSITIONED_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_JUMP_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_JUMP_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_PROPERTYCHANGED_INT_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_PROPERTYCHANGED_INT_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_PROPERTYCHANGED_UINT_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_PROPERTYCHANGED_UINT_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_PROPERTYCHANGED_FLOAT_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_PROPERTYCHANGED_FLOAT_NTF(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_CASTING_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_CASTING_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_CASTING_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_CASTING_ACK(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_CASTING_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_CASTING_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_CASTED_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_CASTED_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_SKILL_COOLDOWN_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_SKILL_COOLDOWN_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_PASSIVE_SKILL_ACTIVE(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_PASSIVE_SKILL_ACTIVE(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_ADD_BUFF_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_ADD_BUFF_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_UPDATE_BUFF_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_UPDATE_BUFF_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_DEL_BUFF_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_DEL_BUFF_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_UPDATED_EQUIPMENT_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_UPDATED_EQUIPMENT_NTF(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_LIST_EQUIPMENT_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_LIST_EQUIPMENT_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_LIST_EQUIPMENT_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_LIST_EQUIPMENT_ACK(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_LIST_EQUIPMENT_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_LIST_EQUIPMENT_NTF(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_DROP_EQUIPMENT_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_DROP_EQUIPMENT_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_DROP_EQUIPMENT_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_DROP_EQUIPMENT_ACK(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_USE_CONSUME_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_USE_CONSUME_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_USE_CONSUME_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_USE_CONSUME_ACK(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_MOVE_EQUIPMENT_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_MOVE_EQUIPMENT_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_MOVE_EQUIPMENT_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_MOVE_EQUIPMENT_ACK(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_MOVE_EQUIPMENT_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_MOVE_EQUIPMENT_NTF(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_QUERY_OPERATOR_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_QUERY_OPERATOR_REQ(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_CHANGE_TABLE_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_CHANGE_TABLE_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_CHANGE_TABLE_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_CHANGE_TABLE_ACK(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_REMOVE_QTE_BUFF_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_REMOVE_QTE_BUFF_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_REMOVE_QTE_BUFF_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_REMOVE_QTE_BUFF_ACK(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_LOCK_PLAYER_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_LOCK_PLAYER_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_WINNER_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_WINNER_NTF(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_SPEAK_NTF(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_SPEAK_NTF(void *pHost, CNetData* poNetData);
int EncodeCLI_GS_TALK_REQ(void *pHost, CNetData* poNetData);
int DecodeCLI_GS_TALK_REQ(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_TALK_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_TALK_ACK(void *pHost, CNetData* poNetData);
int EncodeGS_CLI_NOTIFY_ACK(void *pHost, CNetData* poNetData);
int DecodeGS_CLI_NOTIFY_ACK(void *pHost, CNetData* poNetData);

/*Encode and Decode Function Declare End*/
typedef int (*EnFuncCBnFGameProto)(void *pHost, CNetData* poNetData);
typedef int (*DeFuncCBnFGameProto)(void *pHost, CNetData* poNetData);
class CBnFGameProto : public CSDProtocol
{
public:
	CBnFGameProto();
	virtual ~CBnFGameProto();
    virtual int Encode(int iMsgID, void *pHost, char *pNet, int iNetSize);
    virtual int Decode(int iMsgID, char *pNet,int iNetSize, void *pHost, int iHostSize);
	int GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCBnFGameProto FindEncodeFunc(int iMsgID);
	DeFuncCBnFGameProto FindDecodeFunc(int iMsgID);
	bool Init();
private:
	EnFuncCBnFGameProto m_EncodeFuncArray[BNFGAMEPROTO_MSGID_RANGE];
	DeFuncCBnFGameProto m_DecodeFuncArray[BNFGAMEPROTO_MSGID_RANGE];
	CNetData m_oData;
	int m_iMaxPkgSize;
};
#endif
