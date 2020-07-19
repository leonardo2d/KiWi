#ifndef KIWI_WIDGETS_EDITBOX_H
#define KIWI_WIDGETS_EDITBOX_H

/**
 * \file KW_editbox.h
 * 
 * Defines functions related to creating and managing edit boxes
 */

#include "kiwi/core/widget.h"
#include "kiwi/widgets/widgets-export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   Creates an editbox widget.
 * \details The editbox uses tiles in the third column of the tileset
 * \param   gui The KW_GUI instance that will hold this widget.
 * \param   parent The parent widget of this widget.
 * \param   text The initial text for this editbox
 * \param   geometry The relative geometry of this frame.
 * \return  The editbox instance.
 */
extern KIWI_WIDGETS_EXPORT KW_Widget * KW_CreateEditbox(KW_GUI * gui, KW_Widget * parent,
                                             const char * text,
                                             const KW_Rect * geometry);

/**
 * \brief Specifies the editbox text
 * \details Cursor is reset
 * \param widget the editbox widget to change
 * \param text the text to set
 */
extern KIWI_WIDGETS_EXPORT void KW_SetEditboxText(KW_Widget * widget, const char * text);

/**
 * \brief Returns the text in the editbox
 * \param widget The editbox instance to return text from
 * \return the text in the editbox
 */
extern KIWI_WIDGETS_EXPORT const char * KW_GetEditboxText(KW_Widget * widget);

/**
 * \brief Specify, in terms of characters, the position of the cursor
 * \param widget the edtibox widget to change
 * \param pos the cursor position
 */
extern KIWI_WIDGETS_EXPORT void KW_SetEditboxCursorPosition(KW_Widget * widget,
                                                 unsigned int pos);

/**
 * \brief Returns, in terms of characters, where de cursor is
 * \param widget the editbox widget to return the cursor from
 * \return the cursor position
 */
extern KIWI_WIDGETS_EXPORT unsigned int KW_GetEditboxCursorPosition(KW_Widget * widget);

/**
 * \brief Changes the current font used to render edtibox text.
 * \param widget The editbox widget to change font
 * \param font the KW_Font instance to use
 */
extern KIWI_WIDGETS_EXPORT void KW_SetEditboxFont(KW_Widget * widget, KW_Font * font);

/**
 * \brief Returns the current KW_Font being used in the editbox
 * \param widget the editbox widget to get the font from
 * \return the font being used
 */
extern KIWI_WIDGETS_EXPORT KW_Font * KW_GetEditboxFont(KW_Widget * widget);

/**
 * \brief   Gets the associated text color with an editbox.
 * \param   widget The editbox widget.
 * \returns The KW_Color associated with this editbox text.
 * \details Note that if no color was set, it will return the KW_GUI color
 * \sa KW_WasEditboxTextColorSet
 * \sa KW_SetEditboxTextColor
 */
extern KIWI_WIDGETS_EXPORT KW_Color KW_GetEditboxTextColor(KW_Widget * widget);

/**
 * \brief   Check if the editbox is using its own color or the default one
 * \param   widget The editbox widget.
 * \returns KW_TRUE if the color was set or KW_FALSE otherwise
 * \sa KW_GetEditboxTextColor
 * \sa KW_SetEditboxTextColor
 */
extern KIWI_WIDGETS_EXPORT KW_bool KW_WasEditboxTextColorSet(KW_Widget * widget);

/**
 * \brief   Sets the color of the editbox text.
 * \param   widget The editbox instance.
 * \param   color The color to assign to the editbox.
 */
extern KIWI_WIDGETS_EXPORT void KW_SetEditboxTextColor(KW_Widget * widget, KW_Color color);

#ifdef __cplusplus
}
#endif

#endif
