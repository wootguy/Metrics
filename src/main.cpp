#include "meta_init.h"
#include <string>
#include "misc_utils.h"
#include <map>
#include <set>
#include "main.h"
#include <map>
#include <vector>
#include <algorithm>

// Description of plugin
plugin_info_t Plugin_info = {
	META_INTERFACE_VERSION,	// ifvers
	"Metrics",	// name
	"1.0",	// version
	__DATE__,	// date
	"w00tguy",	// author
	"https://github.com/wootguy/",	// url
	"METRICS",	// logtag, all caps please
	PT_ANYTIME,	// (when) loadable
	PT_ANYPAUSE,	// (when) unloadable
};

map<const char*, int> g_call_counts;

bool cmp(pair<const char*, int>& a, pair<const char*, int>& b) {
	return a.second > b.second;
}

const char* format_int(int v) {
	if (v > 1000*1000) {
		return UTIL_VarArgs("%.1f M", v/(1000.0f*1000.0f));
	} else if (v > 1000) {
		return UTIL_VarArgs("%.1f K", v / 1000.0f);
	}
	return UTIL_VarArgs("%d", v);
}

set<string> lagHooks = {
	/*
	"DLL_AddToFullPack",
	// 40% of calls ^
	"ENG_CheckVisibility",
	"ENG_IndexOfEdict",
	// 80% of calls ^
	"DLL_Think",
	"ENG_PEntityOfEntIndex",
	"ENG_EntOffsetOfPEntity",
	"DLL_SetAbsBox",
	// 95% of calls ^
	"NEWDLL_ShouldCollide",
	"ENG_GetCurrentPlayer",
	"ENG_GetModelPtr",
	"ENG_RandomLong",
	"ENG_FindClientInPVS",
	"ENG_MakeVectors",
	"DLL_Touch",
	"ENG_TraceLine",
	"ENG_DeltaUnsetFieldByIndex",
	// 99% of calls ^
	"ENG_FindEntityInSphere",
	"ENG_CVarGetFloat",
	"ENG_SetOrigin",
	"ENG_ModelIndex",
	"ENG_InfoKeyValue",
	"ENG_FindEntityByString",
	"ENG_GetInfoKeyBuffer"	
	// 99.6% of calls ^
	*/
};

void print_metrics() {
	vector<pair<const char*, int> > sortedPairs;

	int total = 0;
	int real_total = 0;
	for (auto& iter : g_call_counts) {
		sortedPairs.push_back(iter);
		total += iter.second;
	}

	real_total = total;

	// Sort using comparator function
	std::sort(sortedPairs.begin(), sortedPairs.end(), cmp);

	println("\nHook call counts\n----------------");

	int other_total = 0;
	int other_types = 0;

	int exclude_total = 0;
	int exclude_types = 0;

	for (auto& iter : sortedPairs) {
		float percent = ((float)iter.second / (float)total) * 100;

		if (lagHooks.find(iter.first) != lagHooks.end()) {
			exclude_total += iter.second;
			exclude_types++;
			total -= iter.second;
			continue;
		}
	}

	for (auto& iter : sortedPairs) {
		float percent = ((float)iter.second / (float)total) * 100;

		if (lagHooks.find(iter.first) != lagHooks.end()) {
			continue;
		}

		if (percent < 0.1) {
			other_total += iter.second;
			other_types++;
			continue;
		}

		println("%-32s %8s (%.1f%%)", iter.first, format_int(iter.second), percent);
	}

	
	{
		float percent = ((float)other_total / (float)total) * 100;
		string otherText = UTIL_VarArgs("%d other hooks", other_types);
		println("%-32s %8s (%.1f%%)", otherText.c_str(), format_int(other_total), percent);
	}

	println("\n%-32s %8s", "Total", format_int(total));

	/*
	{
		float percent = ((float)exclude_total / (float)real_total) * 100;
		string otherText = UTIL_VarArgs("%d excluded hooks", exclude_types);
		println("\n%-32s %8s (%.2f%%)", otherText.c_str(), format_int(exclude_total), percent);
	}
	*/

	println("");
}

void metrics_reset() {
	g_call_counts.clear();
}

//
// DLL FUNCS
//

void DLL_ClientCommand(edict_t* plr) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_GameInit(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int DLL_Spawn(edict_t* pent) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DLL_Think(edict_t* pent) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_Use(edict_t* pentUsed, edict_t* pentOther) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_Touch(edict_t* pentTouched, edict_t* pentOther) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_Blocked(edict_t* pentBlocked, edict_t* pentOther) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_KeyValue(edict_t* pentKeyvalue, KeyValueData* pkvd) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_Save(edict_t* pent, SAVERESTOREDATA* pSaveData) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int DLL_Restore(edict_t* pent, SAVERESTOREDATA* pSaveData, int globalEntity) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DLL_SetAbsBox(edict_t* pent) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_SaveWriteFields(SAVERESTOREDATA*, const char*, void*, TYPEDESCRIPTION*, int) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_SaveReadFields(SAVERESTOREDATA*, const char*, void*, TYPEDESCRIPTION*, int) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_SaveGlobalState(SAVERESTOREDATA*) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_RestoreGlobalState(SAVERESTOREDATA*) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_ResetGlobalState(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

qboolean DLL_ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DLL_ClientDisconnect(edict_t* pEntity) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_ClientKill(edict_t* pEntity) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_ClientPutInServer(edict_t* pEntity) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_ClientUserInfoChanged(edict_t* pEntity, char* infobuffer) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_ServerDeactivate(void) {
	metrics_reset();
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_PlayerPreThink(edict_t* pEntity) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_PlayerPostThink(edict_t* pEntity) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_StartFrame(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_ParmsNewLevel(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_ParmsChangeLevel(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

const char* DLL_GetGameDescription(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DLL_PlayerCustomization(edict_t* pEntity, customization_t* pCustom) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_SpectatorConnect(edict_t* pEntity) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_SpectatorDisconnect(edict_t* pEntity) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_SpectatorThink(edict_t* pEntity) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_Sys_Error(const char* error_string) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_PM_Move(struct playermove_s* ppmove, qboolean server) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_PM_Init(struct playermove_s* ppmove) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

char DLL_PM_FindTextureType(char* name) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DLL_SetupVisibility(struct edict_s* pViewEntity, struct edict_s* pClient, unsigned char** pvs, unsigned char** pas) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_UpdateClientData(const struct edict_s* ent, int sendweapons, struct clientdata_s* cd) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int DLL_AddToFullPack(struct entity_state_s* state, int e, edict_t* ent, edict_t* host, int hostflags, int player, unsigned char* pSet) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DLL_CreateBaseline(int player, int eindex, struct entity_state_s* baseline, struct edict_s* entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_RegisterEncoders(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int DLL_GetWeaponData(struct edict_s* player, struct weapon_data_s* info) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DLL_CmdStart(const edict_t* player, const struct usercmd_s* cmd, unsigned int random_seed) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void DLL_CmdEnd(const edict_t* player) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int DLL_ConnectionlessPacket(const struct netadr_s* net_from, const char* args, char* response_buffer, int* response_buffer_size) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int DLL_GetHullBounds(int hullnumber, float* mins, float* maxs) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DLL_CreateInstancedBaselines(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int DLL_InconsistentFile(const struct edict_s* player, const char* filename, char* disconnect_message) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int DLL_AllowLagCompensation(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

//
// NEW DLL FUNCS
//

void NEW_OnFreeEntPrivateData(edict_t* pEnt) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void NEW_GameShutdown(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int	NEW_ShouldCollide(edict_t* pentTouched, edict_t* pentOther) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void NEW_CvarValue(const edict_t* pEnt, const char* value) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void NEW_CvarValue2(const edict_t* pEnt, int requestID, const char* cvarName, const char* value) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

//
// ENGINE FUNCS
//

int ENG_PrecacheModel(char* s) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_PrecacheSound(char* s) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_SetModel(edict_t* e, const char* m) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_ModelIndex(const char* m) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_ModelFrames(int modelIndex) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_SetSize(edict_t* e, const float* rgflMin, const float* rgflMax) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ChangeLevel(char* s1, char* s2) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_GetSpawnParms(edict_t* ent) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_SaveSpawnParms(edict_t* ent) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

float ENG_VecToYaw(const float* rgflVector) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_VecToAngles(const float* rgflVectorIn, float* rgflVectorOut) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_MoveToOrigin(edict_t* ent, const float* pflGoal, float dist, int iMoveType) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ChangeYaw(edict_t* ent) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ChangePitch(edict_t* ent) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

edict_t* ENG_FindEntityByString(edict_t* pEdictStartSearchAfter, const char* pszField, const char* pszValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_GetEntityIllum(edict_t* pEnt) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

edict_t* ENG_FindEntityInSphere(edict_t* pEdictStartSearchAfter, const float* org, float rad) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

edict_t* ENG_FindClientInPVS(edict_t* pEdict) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

edict_t* ENG_EntitiesInPVS(edict_t* pplayer) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_MakeVectors(const float* rgflVector) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_AngleVectors(const float* rgflVector, float* forward, float* right, float* up) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

edict_t* ENG_CreateEntity(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_RemoveEntity(edict_t* e) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

edict_t* ENG_CreateNamedEntity(int className) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_MakeStatic(edict_t* ent) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_EntIsOnFloor(edict_t* e) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_DropToFloor(edict_t* e) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_WalkMove(edict_t* ent, float yaw, float dist, int iMode) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_SetOrigin(edict_t* e, const float* rgflOrigin) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_EmitSound(edict_t* entity, int channel, const char* sample, /*int*/float volume, float attenuation, int fFlags, int pitch) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_EmitAmbientSound(edict_t* entity, float* pos, const char* samp, float vol, float attenuation, int fFlags, int pitch) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_TraceLine(const float* v1, const float* v2, int fNoMonsters, edict_t* pentToSkip, TraceResult* ptr) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_TraceToss(edict_t* pent, edict_t* pentToIgnore, TraceResult* ptr) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_TraceMonsterHull(edict_t* pEdict, const float* v1, const float* v2, int fNoMonsters, edict_t* pentToSkip, TraceResult* ptr) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_TraceHull(const float* v1, const float* v2, int fNoMonsters, int hullNumber, edict_t* pentToSkip, TraceResult* ptr) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_TraceModel(const float* v1, const float* v2, int hullNumber, edict_t* pent, TraceResult* ptr) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

const char* ENG_TraceTexture(edict_t* pTextureEntity, const float* v1, const float* v2) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_TraceSphere(const float* v1, const float* v2, int fNoMonsters, float radius, edict_t* pentToSkip, TraceResult* ptr) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_GetAimVector(edict_t* ent, float speed, float* rgflReturn) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ServerCommand(char* str) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ServerExecute(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ClientCommand(edict_t* pEdict, char* szFmt, ...) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ParticleEffect(const float* org, const float* dir, float color, float count) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_LightStyle(int style, char* val) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_DecalIndex(const char* name) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_PointContents(const float* rgflVector) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_MessageEnd(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_WriteByte(int iValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_WriteChar(int iValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_WriteShort(int iValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_WriteLong(int iValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_WriteAngle(float flValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_WriteCoord(float flValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_WriteString(const char* sz) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_WriteEntity(int iValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_CVarRegister(cvar_t* pCvar) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

float ENG_CVarGetFloat(const char* szVarName) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

const char* ENG_CVarGetString(const char* szVarName) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_CVarSetFloat(const char* szVarName, float flValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_CVarSetString(const char* szVarName, const char* szValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_AlertMessage(ALERT_TYPE atype, char* szFmt, ...) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_EngineFprintf(void* pfile, char* szFmt, ...) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void* ENG_PvAllocEntPrivateData(edict_t* pEdict, int32 cb) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void* ENG_PvEntPrivateData(edict_t* pEdict) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_FreeEntPrivateData(edict_t* pEdict) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

const char* ENG_SzFromIndex(int iString) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_AllocString(const char* szValue) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

struct entvars_s* ENG_GetVarsOfEnt(edict_t* pEdict) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

edict_t* ENG_PEntityOfEntOffset(int iEntOffset) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_EntOffsetOfPEntity(const edict_t* pEdict) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_IndexOfEdict(const edict_t* pEdict) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

edict_t* ENG_PEntityOfEntIndex(int iEntIndex) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

edict_t* ENG_FindEntityByVars(struct entvars_s* pvars) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void* ENG_GetModelPtr(edict_t* pEdict) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_RegUserMsg(const char* pszName, int iSize) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_AnimationAutomove(const edict_t* pEdict, float flTime) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_GetBonePosition(const edict_t* pEdict, int iBone, float* rgflOrigin, float* rgflAngles) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

uint32 ENG_FunctionFromName(const char* pName) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

const char* ENG_NameForFunction(uint32 function) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_ClientPrintf(edict_t* pEdict, PRINT_TYPE ptype, const char* szMsg) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ServerPrint(const char* szMsg) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

const char* ENG_Cmd_Args(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

const char* ENG_Cmd_Argv(int argc) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_Cmd_Argc(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_GetAttachment(const edict_t* pEdict, int iAttachment, float* rgflOrigin, float* rgflAngles) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_CRC32_Init(CRC32_t* pulCRC) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_CRC32_ProcessBuffer(CRC32_t* pulCRC, void* p, int len) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_CRC32_ProcessByte(CRC32_t* pulCRC, unsigned char ch) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

CRC32_t ENG_CRC32_Final(CRC32_t pulCRC) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int32 ENG_RandomLong(int32  lLow, int32  lHigh) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

float ENG_RandomFloat(float flLow, float flHigh) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_SetView(const edict_t* pClient, const edict_t* pViewent) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

float ENG_Time(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_CrosshairAngle(const edict_t* pClient, float pitch, float yaw) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

byte* ENG_LoadFileForMe(char* filename, int* pLength) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_FreeFile(void* buffer) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_EndSection(const char* pszSectionName) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_CompareFileTime(char* filename1, char* filename2, int* iCompare) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_GetGameDir(char* szGetGameDir) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_Cvar_RegisterVariable(cvar_t* variable) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_FadeClientVolume(const edict_t* pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_SetClientMaxspeed(const edict_t* pEdict, float fNewMaxspeed) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

edict_t* ENG_CreateFakeClient(const char* netname) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_RunPlayerMove(edict_t* fakeclient, const float* viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_NumberOfEntities(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

char* ENG_GetInfoKeyBuffer(edict_t* e) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

char* ENG_InfoKeyValue(char* infobuffer, char* key) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_SetKeyValue(char* infobuffer, char* key, char* value) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_SetClientKeyValue(int clientIndex, char* infobuffer, char* key, char* value) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_IsMapValid(char* filename) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_StaticDecal(const float* origin, int decalIndex, int entityIndex, int modelIndex) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_PrecacheGeneric(char* s) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_GetPlayerUserId(edict_t* e) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_BuildSoundMsg(edict_t* entity, int channel, const char* sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float* pOrigin, edict_t* ed) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_IsDedicatedServer(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

cvar_t* ENG_CVarGetPointer(const char* szVarName) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

unsigned int ENG_GetPlayerWONId(edict_t* e) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_Info_RemoveKey(char* s, const char* key) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

const char* ENG_GetPhysicsKeyValue(const edict_t* pClient, const char* key) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_SetPhysicsKeyValue(const edict_t* pClient, const char* key, const char* value) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

const char* ENG_GetPhysicsInfoString(const edict_t* pClient) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

unsigned short ENG_PrecacheEvent(int type, const char* psz) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_PlaybackEvent(int flags, const edict_t* pInvoker, unsigned short eventindex, float delay, float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

unsigned char* ENG_SetFatPVS(float* org) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

unsigned char* ENG_SetFatPAS(float* org) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_CheckVisibility(const edict_t* entity, unsigned char* pset) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_DeltaSetField(struct delta_s* pFields, const char* fieldname) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_DeltaUnsetField(struct delta_s* pFields, const char* fieldname) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_DeltaAddEncoder(char* name, void (*conditionalencode)(struct delta_s* pFields, const unsigned char* from, const unsigned char* to)) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_GetCurrentPlayer(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_CanSkipPlayer(const edict_t* player) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_DeltaFindField(struct delta_s* pFields, const char* fieldname) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_DeltaSetFieldByIndex(struct delta_s* pFields, int fieldNumber) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_DeltaUnsetFieldByIndex(struct delta_s* pFields, int fieldNumber) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_SetGroupMask(int mask, int op) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_CreateInstancedBaseline(int classname, struct entity_state_s* baseline) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_Cvar_DirectSet(struct cvar_s* var, char* value) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ForceUnmodified(FORCE_TYPE type, float* mins, float* maxs, const char* filename) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_GetPlayerStats(const edict_t* pClient, int* ping, int* packet_loss) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_AddServerCommand(char* cmd_name, void (*function) (void)) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

qboolean ENG_Voice_GetClientListening(int iReceiver, int iSender) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

qboolean ENG_Voice_SetClientListening(int iReceiver, int iSender, qboolean bListen) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

const char* ENG_GetPlayerAuthId(edict_t* e) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

sequenceEntry_s* ENG_SequenceGet(const char* fileName, const char* entryName) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

sentenceEntry_s* ENG_SequencePickSentence(const char* groupName, int pickMethod, int* picked) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_GetFileSize(char* filename) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

unsigned int ENG_GetApproxWavePlayLen(const char* filepath) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_IsCareerMatch(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int ENG_GetLocalizedStringLength(const char* label) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_RegisterTutorMessageShown(int mid) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_GetTimesTutorMessageShown(int mid) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ENG_ProcessTutorMessageDecayBuffer(int* buffer, int bufferLength) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ConstructTutorMessageDecayBuffer(int* buffer, int bufferLength) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_ResetTutorMessageDecayData(void) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_QueryClientCvarValue(const edict_t* player, const char* cvarName) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

void ENG_QueryClientCvarValue2(const edict_t* player, const char* cvarName, int requestID) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META(MRES_IGNORED);
}

int ENG_EngCheckParm(const char* pchCmdLineToken, char** pchNextVal) {
	g_call_counts[__FUNCTION__]++;
	RETURN_META_VALUE(MRES_IGNORED, 0);
}


void PluginInit() {
	g_dll_hooks.pfnClientCommand = DLL_ClientCommand;

	g_newdll_hooks.pfnCvarValue = NEW_CvarValue;
	g_newdll_hooks.pfnCvarValue2 = NEW_CvarValue2;
	g_newdll_hooks.pfnGameShutdown = NEW_GameShutdown;
	g_newdll_hooks.pfnOnFreeEntPrivateData = NEW_OnFreeEntPrivateData;
	g_newdll_hooks.pfnShouldCollide = NEW_ShouldCollide;

	g_dll_hooks.pfnGameInit = DLL_GameInit;
	g_dll_hooks.pfnSpawn = DLL_Spawn;
	g_dll_hooks.pfnThink = DLL_Think;
	g_dll_hooks.pfnUse = DLL_Use;
	g_dll_hooks.pfnTouch = DLL_Touch;
	g_dll_hooks.pfnBlocked = DLL_Blocked;
	g_dll_hooks.pfnKeyValue = DLL_KeyValue;
	g_dll_hooks.pfnSave = DLL_Save;
	g_dll_hooks.pfnRestore = DLL_Restore;
	g_dll_hooks.pfnSetAbsBox = DLL_SetAbsBox;
	g_dll_hooks.pfnSaveWriteFields = DLL_SaveWriteFields;
	g_dll_hooks.pfnSaveReadFields = DLL_SaveReadFields;
	g_dll_hooks.pfnSaveGlobalState = DLL_SaveGlobalState;
	g_dll_hooks.pfnRestoreGlobalState = DLL_RestoreGlobalState;
	g_dll_hooks.pfnResetGlobalState = DLL_ResetGlobalState;
	g_dll_hooks.pfnClientConnect = DLL_ClientConnect;
	g_dll_hooks.pfnClientDisconnect = DLL_ClientDisconnect;
	g_dll_hooks.pfnClientKill = DLL_ClientKill;
	g_dll_hooks.pfnClientPutInServer = DLL_ClientPutInServer;
	g_dll_hooks.pfnClientCommand = DLL_ClientCommand;
	g_dll_hooks.pfnClientUserInfoChanged = DLL_ClientUserInfoChanged;
	g_dll_hooks.pfnServerActivate = DLL_ServerActivate;
	g_dll_hooks.pfnServerDeactivate = DLL_ServerDeactivate;
	g_dll_hooks.pfnPlayerPreThink = DLL_PlayerPreThink;
	g_dll_hooks.pfnPlayerPostThink = DLL_PlayerPostThink;
	g_dll_hooks.pfnStartFrame = DLL_StartFrame;
	g_dll_hooks.pfnParmsNewLevel = DLL_ParmsNewLevel;
	g_dll_hooks.pfnParmsChangeLevel = DLL_ParmsChangeLevel;
	g_dll_hooks.pfnGetGameDescription = DLL_GetGameDescription;
	g_dll_hooks.pfnPlayerCustomization = DLL_PlayerCustomization;
	g_dll_hooks.pfnSpectatorConnect = DLL_SpectatorConnect;
	g_dll_hooks.pfnSpectatorDisconnect = DLL_SpectatorDisconnect;
	g_dll_hooks.pfnSpectatorThink = DLL_SpectatorThink;
	g_dll_hooks.pfnSys_Error = DLL_Sys_Error;
	g_dll_hooks.pfnPM_Move = DLL_PM_Move;
	g_dll_hooks.pfnPM_Init = DLL_PM_Init;
	g_dll_hooks.pfnPM_FindTextureType = DLL_PM_FindTextureType;
	g_dll_hooks.pfnSetupVisibility = DLL_SetupVisibility;
	g_dll_hooks.pfnUpdateClientData = DLL_UpdateClientData;
	g_dll_hooks.pfnAddToFullPack = DLL_AddToFullPack;
	g_dll_hooks.pfnCreateBaseline = DLL_CreateBaseline;
	g_dll_hooks.pfnRegisterEncoders = DLL_RegisterEncoders;
	g_dll_hooks.pfnGetWeaponData = DLL_GetWeaponData;
	g_dll_hooks.pfnCmdStart = DLL_CmdStart;
	g_dll_hooks.pfnCmdEnd = DLL_CmdEnd;
	g_dll_hooks.pfnConnectionlessPacket = DLL_ConnectionlessPacket;
	g_dll_hooks.pfnGetHullBounds = DLL_GetHullBounds;
	g_dll_hooks.pfnCreateInstancedBaselines = DLL_CreateInstancedBaselines;
	g_dll_hooks.pfnInconsistentFile = DLL_InconsistentFile;
	g_dll_hooks.pfnAllowLagCompensation = DLL_AllowLagCompensation;

	g_engine_hooks.pfnPrecacheModel = ENG_PrecacheModel;
	g_engine_hooks.pfnPrecacheSound = ENG_PrecacheSound;
	g_engine_hooks.pfnSetModel = ENG_SetModel;
	g_engine_hooks.pfnModelIndex = ENG_ModelIndex;
	g_engine_hooks.pfnModelFrames = ENG_ModelFrames;
	g_engine_hooks.pfnSetSize = ENG_SetSize;
	g_engine_hooks.pfnChangeLevel = ENG_ChangeLevel;
	g_engine_hooks.pfnGetSpawnParms = ENG_GetSpawnParms;
	g_engine_hooks.pfnSaveSpawnParms = ENG_SaveSpawnParms;
	g_engine_hooks.pfnVecToYaw = ENG_VecToYaw;
	g_engine_hooks.pfnVecToAngles = ENG_VecToAngles;
	g_engine_hooks.pfnMoveToOrigin = ENG_MoveToOrigin;
	g_engine_hooks.pfnChangeYaw = ENG_ChangeYaw;
	g_engine_hooks.pfnChangePitch = ENG_ChangePitch;
	g_engine_hooks.pfnFindEntityByString = ENG_FindEntityByString;
	g_engine_hooks.pfnGetEntityIllum = ENG_GetEntityIllum;
	g_engine_hooks.pfnFindEntityInSphere = ENG_FindEntityInSphere;
	g_engine_hooks.pfnFindClientInPVS = ENG_FindClientInPVS;
	g_engine_hooks.pfnEntitiesInPVS = ENG_EntitiesInPVS;
	g_engine_hooks.pfnMakeVectors = ENG_MakeVectors;
	g_engine_hooks.pfnAngleVectors = ENG_AngleVectors;
	g_engine_hooks.pfnCreateEntity = ENG_CreateEntity;
	g_engine_hooks.pfnRemoveEntity = ENG_RemoveEntity;
	g_engine_hooks.pfnCreateNamedEntity = ENG_CreateNamedEntity;
	g_engine_hooks.pfnMakeStatic = ENG_MakeStatic;
	g_engine_hooks.pfnEntIsOnFloor = ENG_EntIsOnFloor;
	g_engine_hooks.pfnDropToFloor = ENG_DropToFloor;
	g_engine_hooks.pfnWalkMove = ENG_WalkMove;
	g_engine_hooks.pfnSetOrigin = ENG_SetOrigin;
	g_engine_hooks.pfnEmitSound = ENG_EmitSound;
	g_engine_hooks.pfnEmitAmbientSound = ENG_EmitAmbientSound;
	g_engine_hooks.pfnTraceLine = ENG_TraceLine;
	g_engine_hooks.pfnTraceToss = ENG_TraceToss;
	g_engine_hooks.pfnTraceMonsterHull = ENG_TraceMonsterHull;
	g_engine_hooks.pfnTraceHull = ENG_TraceHull;
	g_engine_hooks.pfnTraceModel = ENG_TraceModel;
	g_engine_hooks.pfnTraceTexture = ENG_TraceTexture;
	g_engine_hooks.pfnTraceSphere = ENG_TraceSphere;
	g_engine_hooks.pfnGetAimVector = ENG_GetAimVector;
	g_engine_hooks.pfnServerCommand = ENG_ServerCommand;
	g_engine_hooks.pfnServerExecute = ENG_ServerExecute;
	g_engine_hooks.pfnClientCommand = ENG_ClientCommand;
	g_engine_hooks.pfnParticleEffect = ENG_ParticleEffect;
	g_engine_hooks.pfnLightStyle = ENG_LightStyle;
	g_engine_hooks.pfnDecalIndex = ENG_DecalIndex;
	g_engine_hooks.pfnPointContents = ENG_PointContents;
	g_engine_hooks.pfnMessageBegin = ENG_MessageBegin;
	g_engine_hooks.pfnMessageEnd = ENG_MessageEnd;
	g_engine_hooks.pfnWriteByte = ENG_WriteByte;
	g_engine_hooks.pfnWriteChar = ENG_WriteChar;
	g_engine_hooks.pfnWriteShort = ENG_WriteShort;
	g_engine_hooks.pfnWriteLong = ENG_WriteLong;
	g_engine_hooks.pfnWriteAngle = ENG_WriteAngle;
	g_engine_hooks.pfnWriteCoord = ENG_WriteCoord;
	g_engine_hooks.pfnWriteString = ENG_WriteString;
	g_engine_hooks.pfnWriteEntity = ENG_WriteEntity;
	g_engine_hooks.pfnCVarRegister = ENG_CVarRegister;
	g_engine_hooks.pfnCVarGetFloat = ENG_CVarGetFloat;
	g_engine_hooks.pfnCVarGetString = ENG_CVarGetString;
	g_engine_hooks.pfnCVarSetFloat = ENG_CVarSetFloat;
	g_engine_hooks.pfnCVarSetString = ENG_CVarSetString;
	g_engine_hooks.pfnAlertMessage = ENG_AlertMessage;
	g_engine_hooks.pfnEngineFprintf = ENG_EngineFprintf;
	g_engine_hooks.pfnPvAllocEntPrivateData = ENG_PvAllocEntPrivateData;
	g_engine_hooks.pfnPvEntPrivateData = ENG_PvEntPrivateData;
	g_engine_hooks.pfnFreeEntPrivateData = ENG_FreeEntPrivateData;
	g_engine_hooks.pfnSzFromIndex = ENG_SzFromIndex;
	g_engine_hooks.pfnAllocString = ENG_AllocString;
	g_engine_hooks.pfnGetVarsOfEnt = ENG_GetVarsOfEnt;
	g_engine_hooks.pfnPEntityOfEntOffset = ENG_PEntityOfEntOffset;
	g_engine_hooks.pfnEntOffsetOfPEntity = ENG_EntOffsetOfPEntity;
	g_engine_hooks.pfnIndexOfEdict = ENG_IndexOfEdict;
	g_engine_hooks.pfnPEntityOfEntIndex = ENG_PEntityOfEntIndex;
	g_engine_hooks.pfnFindEntityByVars = ENG_FindEntityByVars;
	g_engine_hooks.pfnGetModelPtr = ENG_GetModelPtr;
	g_engine_hooks.pfnRegUserMsg = ENG_RegUserMsg;
	g_engine_hooks.pfnAnimationAutomove = ENG_AnimationAutomove;
	g_engine_hooks.pfnGetBonePosition = ENG_GetBonePosition;
	g_engine_hooks.pfnFunctionFromName = ENG_FunctionFromName;
	g_engine_hooks.pfnNameForFunction = ENG_NameForFunction;
	g_engine_hooks.pfnClientPrintf = ENG_ClientPrintf;
	g_engine_hooks.pfnServerPrint = ENG_ServerPrint;
	g_engine_hooks.pfnCmd_Args = ENG_Cmd_Args;
	g_engine_hooks.pfnCmd_Argv = ENG_Cmd_Argv;
	g_engine_hooks.pfnCmd_Argc = ENG_Cmd_Argc;
	g_engine_hooks.pfnGetAttachment = ENG_GetAttachment;
	g_engine_hooks.pfnCRC32_Init = ENG_CRC32_Init;
	g_engine_hooks.pfnCRC32_ProcessBuffer = ENG_CRC32_ProcessBuffer;
	g_engine_hooks.pfnCRC32_ProcessByte = ENG_CRC32_ProcessByte;
	g_engine_hooks.pfnCRC32_Final = ENG_CRC32_Final;
	g_engine_hooks.pfnRandomLong = ENG_RandomLong;
	g_engine_hooks.pfnRandomFloat = ENG_RandomFloat;
	g_engine_hooks.pfnSetView = ENG_SetView;
	g_engine_hooks.pfnTime = ENG_Time;
	g_engine_hooks.pfnCrosshairAngle = ENG_CrosshairAngle;
	g_engine_hooks.pfnLoadFileForMe = ENG_LoadFileForMe;
	g_engine_hooks.pfnFreeFile = ENG_FreeFile;
	g_engine_hooks.pfnEndSection = ENG_EndSection;
	g_engine_hooks.pfnCompareFileTime = ENG_CompareFileTime;
	g_engine_hooks.pfnGetGameDir = ENG_GetGameDir;
	g_engine_hooks.pfnCvar_RegisterVariable = ENG_Cvar_RegisterVariable;
	g_engine_hooks.pfnFadeClientVolume = ENG_FadeClientVolume;
	g_engine_hooks.pfnSetClientMaxspeed = ENG_SetClientMaxspeed;
	g_engine_hooks.pfnCreateFakeClient = ENG_CreateFakeClient;
	g_engine_hooks.pfnRunPlayerMove = ENG_RunPlayerMove;
	g_engine_hooks.pfnNumberOfEntities = ENG_NumberOfEntities;
	g_engine_hooks.pfnGetInfoKeyBuffer = ENG_GetInfoKeyBuffer;
	g_engine_hooks.pfnInfoKeyValue = ENG_InfoKeyValue;
	g_engine_hooks.pfnSetKeyValue = ENG_SetKeyValue;
	g_engine_hooks.pfnSetClientKeyValue = ENG_SetClientKeyValue;
	g_engine_hooks.pfnIsMapValid = ENG_IsMapValid;
	g_engine_hooks.pfnStaticDecal = ENG_StaticDecal;
	g_engine_hooks.pfnPrecacheGeneric = ENG_PrecacheGeneric;
	g_engine_hooks.pfnGetPlayerUserId = ENG_GetPlayerUserId;
	g_engine_hooks.pfnBuildSoundMsg = ENG_BuildSoundMsg;
	g_engine_hooks.pfnIsDedicatedServer = ENG_IsDedicatedServer;
	g_engine_hooks.pfnCVarGetPointer = ENG_CVarGetPointer;
	g_engine_hooks.pfnGetPlayerWONId = ENG_GetPlayerWONId;
	g_engine_hooks.pfnInfo_RemoveKey = ENG_Info_RemoveKey;
	g_engine_hooks.pfnGetPhysicsKeyValue = ENG_GetPhysicsKeyValue;
	g_engine_hooks.pfnSetPhysicsKeyValue = ENG_SetPhysicsKeyValue;
	g_engine_hooks.pfnGetPhysicsInfoString = ENG_GetPhysicsInfoString;
	g_engine_hooks.pfnPrecacheEvent = ENG_PrecacheEvent;
	g_engine_hooks.pfnPlaybackEvent = ENG_PlaybackEvent;
	g_engine_hooks.pfnSetFatPVS = ENG_SetFatPVS;
	g_engine_hooks.pfnSetFatPAS = ENG_SetFatPAS;
	g_engine_hooks.pfnCheckVisibility = ENG_CheckVisibility;
	g_engine_hooks.pfnDeltaSetField = ENG_DeltaSetField;
	g_engine_hooks.pfnDeltaUnsetField = ENG_DeltaUnsetField;
	g_engine_hooks.pfnDeltaAddEncoder = ENG_DeltaAddEncoder;
	g_engine_hooks.pfnGetCurrentPlayer = ENG_GetCurrentPlayer;
	g_engine_hooks.pfnCanSkipPlayer = ENG_CanSkipPlayer;
	g_engine_hooks.pfnDeltaFindField = ENG_DeltaFindField;
	g_engine_hooks.pfnDeltaSetFieldByIndex = ENG_DeltaSetFieldByIndex;
	g_engine_hooks.pfnDeltaUnsetFieldByIndex = ENG_DeltaUnsetFieldByIndex;
	g_engine_hooks.pfnSetGroupMask = ENG_SetGroupMask;
	g_engine_hooks.pfnCreateInstancedBaseline = ENG_CreateInstancedBaseline;
	g_engine_hooks.pfnCvar_DirectSet = ENG_Cvar_DirectSet;
	g_engine_hooks.pfnForceUnmodified = ENG_ForceUnmodified;
	g_engine_hooks.pfnGetPlayerStats = ENG_GetPlayerStats;
	g_engine_hooks.pfnAddServerCommand = ENG_AddServerCommand;
	g_engine_hooks.pfnVoice_GetClientListening = ENG_Voice_GetClientListening;
	g_engine_hooks.pfnVoice_SetClientListening = ENG_Voice_SetClientListening;
	g_engine_hooks.pfnGetPlayerAuthId = ENG_GetPlayerAuthId;
	g_engine_hooks.pfnSequenceGet = ENG_SequenceGet;
	g_engine_hooks.pfnSequencePickSentence = ENG_SequencePickSentence;
	g_engine_hooks.pfnGetFileSize = ENG_GetFileSize;
	g_engine_hooks.pfnGetApproxWavePlayLen = ENG_GetApproxWavePlayLen;
	g_engine_hooks.pfnIsCareerMatch = ENG_IsCareerMatch;
	g_engine_hooks.pfnGetLocalizedStringLength = ENG_GetLocalizedStringLength;
	g_engine_hooks.pfnRegisterTutorMessageShown = ENG_RegisterTutorMessageShown;
	g_engine_hooks.pfnGetTimesTutorMessageShown = ENG_GetTimesTutorMessageShown;
	g_engine_hooks.pfnProcessTutorMessageDecayBuffer = ENG_ProcessTutorMessageDecayBuffer;
	g_engine_hooks.pfnConstructTutorMessageDecayBuffer = ENG_ConstructTutorMessageDecayBuffer;
	g_engine_hooks.pfnResetTutorMessageDecayData = ENG_ResetTutorMessageDecayData;
	g_engine_hooks.pfnQueryClientCvarValue = ENG_QueryClientCvarValue;
	g_engine_hooks.pfnQueryClientCvarValue2 = ENG_QueryClientCvarValue2;
	g_engine_hooks.pfnEngCheckParm = ENG_EngCheckParm;

	REG_SVR_COMMAND("metrics", print_metrics);
	REG_SVR_COMMAND("metrics_reset", metrics_reset);
}

void PluginExit() {}