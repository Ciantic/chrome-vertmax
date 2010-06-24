/* 
 * Chrome Vertical maximize extension plugin.
 * http://github.com/Ciantic/chrome-vertmax
 * (C) Jari Pennanen, 2010.
 * Licensed as FreeBSD, see LICENSE.rst
 *
 * This file contains a lot of NPAPI boiler plate code, if you are solely 
 * interested on the method I use for vertical maximizing then jump to 
 * "Do vertical maximize"
 *
 * Following license is for 'npsimple', the NPAPI example which this plugin 
 * is based on:
 * 
 * ***** BEGIN LICENSE BLOCK *****
 * (C)opyright 2008-2009 Aplix Corporation. anselm@aplixcorp.com
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 * ***** END LICENSE BLOCK ***** */

#define WM_CHROME_WIN_CREATED WM_USER+1

#include <stdio.h>
#include <string.h>

#include <windows.h>

#include <npapi.h>
#include <prtypes.h>
#include <npfunctions.h>
#include <npruntime.h>

static NPObject        *so       = NULL;
static NPNetscapeFuncs *npnfuncs = NULL;
static NPP              inst     = NULL;

/* NPN */

static void logmsg(char *fmt, ...) {
	return; // Disabled

	FILE *out = fopen("C:\\Projects\\NPAPI\\chrome-vertmax\\plugin\\Release\\debug.log", "a");
	if(out) {
		va_list args;
		va_start( args, fmt );
		vfprintf( out, fmt, args );
		fprintf( out, "\n" );
		va_end( args );
		fclose(out);
	}
}


static bool
hasMethod(NPObject* obj, NPIdentifier methodName) {
	logmsg("npsimple: hasMethod");
	return true;
}

static bool
invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	logmsg("npsimple: invokeDefault");
	return true;
}

static bool
invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	logmsg("npsimple: invoke");
	int error = 1;
	char *name = npnfuncs->utf8fromidentifier(methodName);
	if(name) {
		if(!strcmp(name, "onWindowCreated")) {

			
			// Do vertical maximize
			Sleep(50); // Window is not active just yet
			HWND active = GetForegroundWindow();
			PostMessage((HWND) active, WM_NCLBUTTONDBLCLK, HTTOP, 0);

			// Returns true
			result->type = NPVariantType_Bool;
			result->value.boolValue = 1;

			// Accept sending the value
			return invokeDefault(obj, args, argCount, result);
		} 
	}
	// aim exception handling
	npnfuncs->setexception(obj, "exception during invocation");
	return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier propertyName) {
	logmsg("npsimple: hasProperty");
	return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
	logmsg("npsimple: getProperty");
	return false;
}

static NPClass npcRefObject = {
	NP_CLASS_STRUCT_VERSION,
	NULL,
	NULL,
	NULL,
	hasMethod,
	invoke,
	invokeDefault,
	hasProperty,
	getProperty,
	NULL,
	NULL,
};

/* NPP */

static NPError
nevv(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved) {
	inst = instance;
	logmsg("npsimple: new");
	return NPERR_NO_ERROR;
}

static NPError
destroy(NPP instance, NPSavedData **save) {
	if(so)
		npnfuncs->releaseobject(so);
	so = NULL;
	logmsg("npsimple: destroy");
	return NPERR_NO_ERROR;
}

static NPError
getValue(NPP instance, NPPVariable variable, void *value) {
	inst = instance;
	switch(variable) {
	default:
		logmsg("npsimple: getvalue - default");
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
		logmsg("npsimple: getvalue - name string");
		*((char **)value) = "VertMax for Windows7";
		break;
	case NPPVpluginDescriptionString:
		logmsg("npsimple: getvalue - description string");
		*((char **)value) = "<a href=\"http://github.com/Ciantic/chrome-vertmax\">VertMax for Windows7</a> plugin.";
		break;
	case NPPVpluginScriptableNPObject:
		logmsg("npsimple: getvalue - scriptable object");
		if(!so)
			so = npnfuncs->createobject(instance, &npcRefObject);
		npnfuncs->retainobject(so);
		*(NPObject **)value = so;
		break;
	case NPPVpluginNeedsXEmbed:
		logmsg("npsimple: getvalue - xembed");
		*((PRBool *)value) = PR_FALSE;
		break;
	}
	return NPERR_NO_ERROR;
}

static NPError /* expected by Safari on Darwin */
handleEvent(NPP instance, void *ev) {
	inst = instance;
	logmsg("npsimple: handleEvent");
	return NPERR_NO_ERROR;
}

static NPError /* expected by Opera */
setWindow(NPP instance, NPWindow* pNPWindow) {
	inst = instance;
	logmsg("npsimple: setWindow");
	return NPERR_NO_ERROR;
}

/* EXPORT */
#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL
NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
	logmsg("npsimple: NP_GetEntryPoints");
	nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
	nppfuncs->newp          = nevv;
	nppfuncs->destroy       = destroy;
	nppfuncs->getvalue      = getValue;
	nppfuncs->event         = handleEvent;
	nppfuncs->setwindow     = setWindow;

	return NPERR_NO_ERROR;
}

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32)(x)) & 0xff00) >> 8)
#endif

NPError OSCALL
NP_Initialize(NPNetscapeFuncs *npnf)
{
	logmsg("npsimple: NP_Initialize");
	if(npnf == NULL)
		return NPERR_INVALID_FUNCTABLE_ERROR;

	if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
		return NPERR_INCOMPATIBLE_VERSION_ERROR;

	npnfuncs = npnf;
	return NPERR_NO_ERROR;
}

NPError
OSCALL NP_Shutdown() {
	logmsg("npsimple: NP_Shutdown");
	return NPERR_NO_ERROR;
}

char *
NP_GetMIMEDescription(void) {
	logmsg("npsimple: NP_GetMIMEDescription");
	return "application/x-vertmax::jari.pennanen@gmail.com";
}

NPError OSCALL /* needs to be present for WebKit based browsers */
NP_GetValue(void *npp, NPPVariable variable, void *value) {
	inst = (NPP)npp;
	return getValue((NPP)npp, variable, value);
}

#ifdef __cplusplus
}
#endif
