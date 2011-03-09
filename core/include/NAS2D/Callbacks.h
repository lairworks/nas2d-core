// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef __NAS_CALLBACKS__
#define __NAS_CALLBACKS__

#if !defined(WIN32)
#include <tr1/functional>
#else
#include <functional>
#endif

// Callback functional types
/**
 * \typedef	Callback_No_Params_No_Return
 * \brief	A callback function binding with no paramters.
 */
typedef std::tr1::function<void(void)> Callback_No_Params_No_Return;

/**
 * \typedef	Callback_1_Int_No_Return
 * \brief	A callback function binding with one integer paramters.
 */
typedef std::tr1::function<void(int)> Callback_1_Int_No_Return;

/**
 * \typedef	Callback_2_Int_No_Return
 * \brief	A callback function binding with two integer paramters.
 */
typedef std::tr1::function<void(int, int)> Callback_2_Int_No_Return;

/**
 * \typedef	Callback_1_String_No_Return
 * \brief	A callback function binding with one string paramter.
 */
typedef std::tr1::function<void(const std::string&)> Callback_1_String_No_Return;

#endif