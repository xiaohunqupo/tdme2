![LOGO](https://raw.githubusercontent.com/andreasdr/tdme2/master/resources/github/tdme2-logo.png)

This is a documentation of supported TDME2-GUI XML elements and their attributes. This document is WIP.

# 1. Primitive nodes

## 1.1. \<element\>
| Attribute name   | Attribute description |
| ---------------- | ----------- |
| background-color |             |
| border           |             |
| border-color     |             |
| focusable        |             |
| height           |             |
| horizontal-align |             |
| id               |             |       
| ignore-events    |             |
| on-mouse-click   |             |
| options          |             |
| show-on          |             |
| vertical-align   |             |
| width            |             |

## 1.2. gradient

## 1.3. images

### 1.3.1. \<frame-buffer\>
        background-color
        height
        id
        width

### 1.3.2. \<image\>
        color-effect-mul
        effect-color-mul
        height
        hide-on
        id
        scale9
        show-on
        src
        vertical-align
        width

### 1.3.3. \<texture\>
        height
        id
        src
        width
       
## 1.4. \<input\>
        disabled
        height
        hide-on
        hint
        id
        image
        maxlength
        name
        on-change
        on-mouse-click
        on-mouse-doubleclick
        options
        show-on
        text
        unit
        width
    
## 1.5. \<layer\>
        background-color
        border
        border-color
        height
        hide-on
        horizontal-align
        id
        padding
        vertical-align
        width

## 1.6. \<layout\>
        alignemnt
        alignment
        background-color
        background-image
        background-image-effect-color-mul
        background-image-scale9
        border
        border-bottom
        border-color
        border-color-bottom
        border-color-top
        border-left
        border-right
        border-top
        flow
        height
        hide-on
        horizontal-align
        id
        overflow-x
        overflow-y
        padding
        padding-bottom
        padding-left
        padding-right
        padding-top
        show-on
        vertical-align
        width

## 1.7. \<panel\>
        background-color
        height
        width

## 1.8. \<scrollarea\>
        * accepts XML within tag
        alignment
        background-color
        border
        focusable
        height
        hide-on
        horizontal-align
        id
        padding
        show-on
        vertical-align
        width

## 1.9. \<space\>
        alignment
        border-bottom
        border-color-bottom
        factor
        height
        hide-on
        horizontal-align
        show-on
        vertical-align
        width

## 1.10. table

## 1.11. text

### 1.11.1. \<text\>
        background-color
        color
        eight
        font
        height
        hide-on
        horizontal-align
        id
        padding-bottom
        padding-left
        padding-right
        padding-top
        show-on
        text
        vertical-align
        width

### 1.11.2. \<styled-text\>
        color
        editable
        font
        height
        horizontal-align
        id
        preformatted
        width

# 2. Compound elements

## 2.1. \<button\>
        background-image
        disabled
        height
        hide-on
        id
        image
        on-mouse-click
        on-mouse-doubleclick
        on-mouse-out
        on-mouse-over
        show-on
        template
        text
        value
        width

## 2.2. \<checkbox\>
        disabled
        height
        id
        image-checked
        image-unchecked
        name
        on-change
        on-mouse-click
        on-mouse-doubleclick
        selected
        size
        text
        value
        width

## 2.3 \<context-menu\>
        * accepts XML within tag
        disabled
        id
        name
        on-change
        on-mouse-click
        on-mouse-doubleclick

## 2.3. dropdown

### 2.3.1. \<dropdown\>
        * accepts XML within tag
        disabled
        height
        id
        image
        on-change
        on-mouse-click
        on-mouse-doubleclick
        padding
        template
        text
        width

### 2.3.2. \<dropdown-option\>
        * accepts XML within tag
        disabled
        id
        on-mouse-click
        on-mouse-doubleclick
        selected
        text
        value

## 2.4. menu

### 2.4.1. \<menu-header\>
        * accepts XML within tag

### 2.4.2. \<menu-header-item\>
        * accepts XML within tag
        disabled
        id
        name
        on-change
        on-mouse-click
        on-mouse-doubleclick
        text

### 2.4.3. \<menu-item\>
        * accepts XML within tag
        disabled
        id
        image
        on-mouse-click
        on-mouse-doubleclick
        shortcut
        text
        value

### 2.4.4. \<menu-separator\>

## 2.5. \<progress-bar\>
        id
        show-on
        text-on
        value
        width

## 2.6. \<radiobutton\>
        disabled
        height
        id
        image
        name
        on-change
        on-mouse-click
        on-mouse-doubleclick
        selected
        template
        text
        value
        width

## 2.7. selectbox

### 2.7.1. \<selectbox\>
        * accepts XML within tag
        boder
        disabled
        height
        id
        name
        on-change
        on-mouse-click
        on-mouse-doubleclick
        options
        width

### 2.7.2. \<selectbox-option\>
        disabled
        id
        image
        on-mouse-click
        on-mouse-doubleclick
        parent-id
        selected
        text
        value

## 2.8. \<slider-vertical\>
        height
        id
        template
        value

## 2.9. \<styled-input\>
        * accepts XML within tag
        color
        disabled
        font
        height
        hide-on
        id
        on-change
        on-mouse-click
        on-mouse-doubleclick
        options
        padding
        preformatted
        show-on
        width

## 2.10. tab

### 2.10.1. \<tab\>
        closeable
        disabled
        id
        image
        on-mouse-click
        on-mouse-doubleclick
        text
        type-color
        value

### 2.10.2. \<tab-content\>
        * accepts XML within tag
        alignment
        horizontal-align
        tab-id
        vertical-align

### 2.10.3. \<tabs\>
        * accepts XML within tag
        alignment
        height
        id
        ignore-events
        width

### 2.10.4. \<tabs-content\>
        * accepts XML within tag
        id
        ignore-events

### 2.10.5. \<tabs-header\>
        * accepts XML within tag
        id
        scrollable

## 2.11. \<template\>
        id
        name
        src
        type
