#
# $Id: Blue.csc,v 1.2 2000/10/12 01:59:28 idiscovery Exp $
#
proc tixPref:SetScheme-Color:Blue {} {

    global tixOption

    set tixOption(bg)           #9090f0
    set tixOption(fg)           black

    set tixOption(dark1_bg)     #8080d0
    set tixOption(dark1_fg)     black
    set tixOption(dark2_bg)     #7070c0
    set tixOption(dark2_fg)     black
    set tixOption(inactive_bg)  #8080da
    set tixOption(inactive_fg)  black

    set tixOption(light1_bg)    #a8a8ff
    set tixOption(light1_fg)    black
    set tixOption(light2_bg)    #c0c0ff
    set tixOption(light2_fg)    black

    set tixOption(active_bg)    $tixOption(dark1_bg)
    set tixOption(active_fg)    $tixOption(fg)
    set tixOption(disabled_fg)  gray25

    set tixOption(input1_bg)    $tixOption(light1_bg)
    set tixOption(input2_bg)    $tixOption(bg)
    set tixOption(output1_bg)   $tixOption(light1_bg)
    set tixOption(output2_bg)   $tixOption(bg)

    set tixOption(select_fg)    white
    set tixOption(select_bg)    black

    set tixOption(selector)	yellow

option add *background 		$tixOption(bg) 10
option add *Background		$tixOption(bg) $tixOption(prioLevel)
option add *background		$tixOption(bg) $tixOption(prioLevel)
option add *Foreground		$tixOption(fg) $tixOption(prioLevel)
option add *foreground		$tixOption(fg) $tixOption(prioLevel)
option add *activeBackground	$tixOption(active_bg) $tixOption(prioLevel)
option add *activeForeground	$tixOption(active_fg) $tixOption(prioLevel)
option add *HighlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *selectBackground	$tixOption(select_bg) $tixOption(prioLevel)
option add *selectForeground	$tixOption(select_fg) $tixOption(prioLevel)
option add *selectBorderWidth	0 $tixOption(prioLevel)
option add *Menu.selectColor		$tixOption(selector) $tixOption(prioLevel)
option add *TixMenu.selectColor		$tixOption(selector) $tixOption(prioLevel)
option add *Menubutton.padY			5 $tixOption(prioLevel)
option add *Button.borderWidth		2 $tixOption(prioLevel)
option add *Button.anchor		c $tixOption(prioLevel)
option add *Checkbutton.selectColor		$tixOption(selector) $tixOption(prioLevel)
option add *Radiobutton.selectColor		$tixOption(selector) $tixOption(prioLevel)
option add *Entry.relief		sunken $tixOption(prioLevel)
option add *Entry.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *Entry.background		$tixOption(input1_bg) $tixOption(prioLevel)
option add *Entry.foreground		black $tixOption(prioLevel)
option add *Entry.insertBackground	black $tixOption(prioLevel)
option add *Label.anchor		w $tixOption(prioLevel)
option add *Label.borderWidth		0 $tixOption(prioLevel)
option add *Listbox.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *Listbox.relief		sunken $tixOption(prioLevel)
option add *Scale.foreground		$tixOption(fg) $tixOption(prioLevel)
option add *Scale.activeForeground	$tixOption(bg) $tixOption(prioLevel)
option add *Scale.background		$tixOption(bg) $tixOption(prioLevel)
option add *Scale.sliderForeground	$tixOption(bg) $tixOption(prioLevel)
option add *Scale.sliderBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *Scrollbar.relief		sunken $tixOption(prioLevel)
option add *Scrollbar.borderWidth		1 $tixOption(prioLevel)
option add *Scrollbar.width			15 $tixOption(prioLevel)
option add *Text.background		$tixOption(input1_bg) $tixOption(prioLevel)
option add *Text.relief		sunken $tixOption(prioLevel)
option add *TixBalloon*background 			#ffff60 $tixOption(prioLevel)
option add *TixBalloon*foreground 			black $tixOption(prioLevel)
option add *TixBalloon.background 			black $tixOption(prioLevel)
option add *TixBalloon*Label.anchor 			w $tixOption(prioLevel)
option add *TixControl*entry.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixControl*entry.background		$tixOption(input1_bg) $tixOption(prioLevel)
option add *TixControl*entry.foreground		black $tixOption(prioLevel)
option add *TixControl*entry.insertBackground	black $tixOption(prioLevel)
option add *TixDirTree*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixDirTree*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirTree*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixDirTree*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirTree*hlist.activeBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirTree*hlist.disabledBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirTree*f1.borderWidth		1 $tixOption(prioLevel)
option add *TixDirTree*f1.relief			sunken $tixOption(prioLevel)
option add *TixDirList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixDirList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirList*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixDirList*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirList*hlist.activeBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirList*hlist.disabledBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirList*f1.borderWidth		1 $tixOption(prioLevel)
option add *TixDirList*f1.relief			sunken $tixOption(prioLevel)
option add *TixScrolledHList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledHList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.activeBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.disabledBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*f1.borderWidth		1 $tixOption(prioLevel)
option add *TixScrolledHList*f1.relief			sunken $tixOption(prioLevel)
option add *TixTree*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixTree*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixTree*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.activeBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.disabledBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*f1.borderWidth		1 $tixOption(prioLevel)
option add *TixTree*f1.relief			sunken $tixOption(prioLevel)
option add *TixFileEntry*Entry.background 		$tixOption(input1_bg) $tixOption(prioLevel)
option add *TixHList.background			$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixHList.activeBackground		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixHList.disabledBackground		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixLabelEntry*entry.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixLabelEntry*entry.background		$tixOption(input1_bg) $tixOption(prioLevel)
option add *TixLabelEntry*entry.foreground		black $tixOption(prioLevel)
option add *TixLabelEntry*entry.insertBackground	black $tixOption(prioLevel)
option add *TixMultiList*Listbox.borderWidth		0 $tixOption(prioLevel)
option add *TixMultiList*Listbox.highlightThickness	0 $tixOption(prioLevel)
option add *TixMultiList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixMultiList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixMultiList*Scrollbar.relief		sunken $tixOption(prioLevel)
option add *TixMultiList*Scrollbar.width		15 $tixOption(prioLevel)
option add *TixMultiList*f1.borderWidth		2 $tixOption(prioLevel)
option add *TixMultiList*f1.relief			sunken $tixOption(prioLevel)
option add *TixMultiList*f1.highlightThickness		2 $tixOption(prioLevel)
option add *TixMDIMenuBar*menubar.relief		raised $tixOption(prioLevel)
option add *TixMDIMenuBar*menubar.borderWidth		2 $tixOption(prioLevel)
option add *TixMDIMenuBar*Menubutton.padY 		2 $tixOption(prioLevel)
option add *TixNoteBook.Background			$tixOption(bg) $tixOption(prioLevel)
option add *TixNoteBook.nbframe.Background		$tixOption(bg) $tixOption(prioLevel)
option add *TixNoteBook.nbframe.backPageColor		$tixOption(bg) $tixOption(prioLevel)
option add *TixNoteBook.nbframe.inactiveBackground	$tixOption(inactive_bg) $tixOption(prioLevel)
option add *TixPanedWindow.handleActiveBg 		$tixOption(active_bg) $tixOption(prioLevel)
option add *TixPanedWindow.seperatorBg    		$tixOption(bg) $tixOption(prioLevel)
option add *TixPanedWindow.handleBg       		$tixOption(dark1_bg) $tixOption(prioLevel)
option add *TixPopupMenu*menubutton.background 	$tixOption(dark1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledHList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledTList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledTList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledTList*tlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledTList*tlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledListBox*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledListBox*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledListBox*listbox.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledListBox*listbox.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledText*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledText*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledWindow*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledWindow*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledWindow.frame.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixTree*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixTree*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.borderWidth		1 $tixOption(prioLevel)
option add *TixComboBox*Entry.highlightBackground		$tixOption(bg) $tixOption(prioLevel)
option add *TixComboBox*Entry.background			$tixOption(input1_bg) $tixOption(prioLevel)
option add *TixComboBox*Entry.foreground			black $tixOption(prioLevel)
option add *TixComboBox*Entry.insertBackground		black $tixOption(prioLevel)
}
proc tixPref:SetScheme-Mono:Blue {} {


    global tixOption

    set tixOption(bg)           lightgray
    set tixOption(fg)           black

    set tixOption(dark1_bg)     gray70
    set tixOption(dark1_fg)     black
    set tixOption(dark2_bg)     gray60
    set tixOption(dark2_fg)     white
    set tixOption(inactive_bg)  lightgray
    set tixOption(inactive_fg)  black

    set tixOption(light1_bg)    gray90
    set tixOption(light1_fg)    white
    set tixOption(light2_bg)    gray95
    set tixOption(light2_fg)    white

    set tixOption(active_bg)    gray90
    set tixOption(active_fg)    $tixOption(fg)
    set tixOption(disabled_fg)  gray55

    set tixOption(input1_bg)    $tixOption(light1_bg)
    set tixOption(input2_bg)    $tixOption(light1_bg)
    set tixOption(output1_bg)   $tixOption(light1_bg)
    set tixOption(output2_bg)   $tixOption(light1_bg)

    set tixOption(select_fg)    white
    set tixOption(select_bg)    black

    set tixOption(selector)	black

option add *background 		$tixOption(bg) 10
option add *Background		$tixOption(bg) $tixOption(prioLevel)
option add *background		$tixOption(bg) $tixOption(prioLevel)
option add *Foreground		$tixOption(fg) $tixOption(prioLevel)
option add *foreground		$tixOption(fg) $tixOption(prioLevel)
option add *activeBackground	$tixOption(active_bg) $tixOption(prioLevel)
option add *activeForeground	$tixOption(active_fg) $tixOption(prioLevel)
option add *HighlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *selectBackground	$tixOption(select_bg) $tixOption(prioLevel)
option add *selectForeground	$tixOption(select_fg) $tixOption(prioLevel)
option add *selectBorderWidth	0 $tixOption(prioLevel)
option add *Menu.selectColor		$tixOption(selector) $tixOption(prioLevel)
option add *TixMenu.selectColor		$tixOption(selector) $tixOption(prioLevel)
option add *Menubutton.padY			5 $tixOption(prioLevel)
option add *Button.borderWidth		2 $tixOption(prioLevel)
option add *Button.anchor		c $tixOption(prioLevel)
option add *Checkbutton.selectColor		$tixOption(selector) $tixOption(prioLevel)
option add *Radiobutton.selectColor		$tixOption(selector) $tixOption(prioLevel)
option add *Entry.relief		sunken $tixOption(prioLevel)
option add *Entry.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *Entry.background		$tixOption(input1_bg) $tixOption(prioLevel)
option add *Entry.foreground		black $tixOption(prioLevel)
option add *Entry.insertBackground	black $tixOption(prioLevel)
option add *Label.anchor		w $tixOption(prioLevel)
option add *Label.borderWidth		0 $tixOption(prioLevel)
option add *Listbox.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *Listbox.relief		sunken $tixOption(prioLevel)
option add *Scale.foreground		$tixOption(fg) $tixOption(prioLevel)
option add *Scale.activeForeground	$tixOption(bg) $tixOption(prioLevel)
option add *Scale.background		$tixOption(bg) $tixOption(prioLevel)
option add *Scale.sliderForeground	$tixOption(bg) $tixOption(prioLevel)
option add *Scale.sliderBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *Scrollbar.relief		sunken $tixOption(prioLevel)
option add *Scrollbar.borderWidth		1 $tixOption(prioLevel)
option add *Scrollbar.width			15 $tixOption(prioLevel)
option add *Text.background		$tixOption(input1_bg) $tixOption(prioLevel)
option add *Text.relief		sunken $tixOption(prioLevel)
option add *TixBalloon*background 			#ffff60 $tixOption(prioLevel)
option add *TixBalloon*foreground 			black $tixOption(prioLevel)
option add *TixBalloon.background 			black $tixOption(prioLevel)
option add *TixBalloon*Label.anchor 			w $tixOption(prioLevel)
option add *TixControl*entry.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixControl*entry.background		$tixOption(input1_bg) $tixOption(prioLevel)
option add *TixControl*entry.foreground		black $tixOption(prioLevel)
option add *TixControl*entry.insertBackground	black $tixOption(prioLevel)
option add *TixDirTree*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixDirTree*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirTree*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixDirTree*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirTree*hlist.activeBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirTree*hlist.disabledBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirTree*f1.borderWidth		1 $tixOption(prioLevel)
option add *TixDirTree*f1.relief			sunken $tixOption(prioLevel)
option add *TixDirList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixDirList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirList*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixDirList*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirList*hlist.activeBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirList*hlist.disabledBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixDirList*f1.borderWidth		1 $tixOption(prioLevel)
option add *TixDirList*f1.relief			sunken $tixOption(prioLevel)
option add *TixScrolledHList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledHList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.activeBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.disabledBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*f1.borderWidth		1 $tixOption(prioLevel)
option add *TixScrolledHList*f1.relief			sunken $tixOption(prioLevel)
option add *TixTree*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixTree*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixTree*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.activeBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.disabledBackground	$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*f1.borderWidth		1 $tixOption(prioLevel)
option add *TixTree*f1.relief			sunken $tixOption(prioLevel)
option add *TixFileEntry*Entry.background 		$tixOption(input1_bg) $tixOption(prioLevel)
option add *TixHList.background			$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixHList.activeBackground		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixHList.disabledBackground		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixLabelEntry*entry.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixLabelEntry*entry.background		$tixOption(input1_bg) $tixOption(prioLevel)
option add *TixLabelEntry*entry.foreground		black $tixOption(prioLevel)
option add *TixLabelEntry*entry.insertBackground	black $tixOption(prioLevel)
option add *TixMultiList*Listbox.borderWidth		0 $tixOption(prioLevel)
option add *TixMultiList*Listbox.highlightThickness	0 $tixOption(prioLevel)
option add *TixMultiList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixMultiList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixMultiList*Scrollbar.relief		sunken $tixOption(prioLevel)
option add *TixMultiList*Scrollbar.width		15 $tixOption(prioLevel)
option add *TixMultiList*f1.borderWidth		2 $tixOption(prioLevel)
option add *TixMultiList*f1.relief			sunken $tixOption(prioLevel)
option add *TixMultiList*f1.highlightThickness		2 $tixOption(prioLevel)
option add *TixMDIMenuBar*menubar.relief		raised $tixOption(prioLevel)
option add *TixMDIMenuBar*menubar.borderWidth		2 $tixOption(prioLevel)
option add *TixMDIMenuBar*Menubutton.padY 		2 $tixOption(prioLevel)
option add *TixNoteBook.Background			$tixOption(bg) $tixOption(prioLevel)
option add *TixNoteBook.nbframe.Background		$tixOption(bg) $tixOption(prioLevel)
option add *TixNoteBook.nbframe.backPageColor		$tixOption(bg) $tixOption(prioLevel)
option add *TixNoteBook.nbframe.inactiveBackground	$tixOption(inactive_bg) $tixOption(prioLevel)
option add *TixPanedWindow.handleActiveBg 		$tixOption(active_bg) $tixOption(prioLevel)
option add *TixPanedWindow.seperatorBg    		$tixOption(bg) $tixOption(prioLevel)
option add *TixPanedWindow.handleBg       		$tixOption(dark1_bg) $tixOption(prioLevel)
option add *TixPopupMenu*menubutton.background 	$tixOption(dark1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledHList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledHList*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledTList*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledTList*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledTList*tlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledTList*tlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledListBox*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledListBox*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledListBox*listbox.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledListBox*listbox.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledText*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledText*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledWindow*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixScrolledWindow*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixScrolledWindow.frame.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*Scrollbar.background		$tixOption(bg) $tixOption(prioLevel)
option add *TixTree*Scrollbar.troughColor		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.highlightBackground	$tixOption(bg) $tixOption(prioLevel)
option add *TixTree*hlist.background		$tixOption(light1_bg) $tixOption(prioLevel)
option add *TixTree*hlist.borderWidth		1 $tixOption(prioLevel)
option add *TixComboBox*Entry.highlightBackground		$tixOption(bg) $tixOption(prioLevel)
option add *TixComboBox*Entry.background			$tixOption(input1_bg) $tixOption(prioLevel)
option add *TixComboBox*Entry.foreground			black $tixOption(prioLevel)
option add *TixComboBox*Entry.insertBackground		black $tixOption(prioLevel)
}
