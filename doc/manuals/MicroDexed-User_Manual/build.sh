#!/bin/bash
#
# sudo apt install wkhtmltopdf pandoc

LCDIMG="../../bin/gen_lcd_screen.sh --imgpath ../../images"

${LCDIMG} --baseimg OperationElements --line1 "42%bracket_open%BANKNAME  %bracket_close%%small_1_inv%%key%" --line2 "42 VOICENAME" --target images/10_OperationElements_1.jpg
${LCDIMG} --baseimg Preset_Long_Press --line1 "00%bracket_open%SONUS1    %bracket_close%%small_1_inv%%key%" --line2 "01 ANLGSYN 1  %note%" --target images/10_SoundBank_1.jpg
${LCDIMG} --baseimg Preset_Setting --line1 "00 SONUS1     %small_1_inv%%key%" --line2 "01%bracket_open%ANLGSYN 1 %bracket_close%" --target images/10_SoundBank_2.jpg
${LCDIMG} --line1 "00%bracket_open%SONUS1    %bracket_close%%small_1_inv%%key%" --line2 "01 ANLGSYN 1  %note%" --target images/10_SoundBank_3.jpg

${LCDIMG} --baseimg Preset_Setting --line1 "Voice Level   %small_1_inv%%key%" --line2 "%block%%block%%block%%block%%block%%block%%block%%block%%block%%block%%full_bar_l1%  100" --target images/10_VoiceLevel_1.jpg

${LCDIMG} --baseimg Preset_Setting --line1 "Panorama      %small_1_inv%%key%" --line2 "         %vert_bar_l3%  %sign-plus%0.6" --target images/10_Panorama_1.jpg

${LCDIMG} --baseimg Preset_Setting --line1 "MIDI Send Voice" --line2 "76%bracket_open%STEPH     %bracket_close%" --target images/10_MIDISendVoice_1.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Send Voice" --line2 "76%bracket_open%STEPH     %bracket_close%" --target images/10_MIDISendVoice_2.jpg
${LCDIMG} --baseimg Preset_Setting --line1 "MIDI Send Voice" --line2 "02%bracket_open%F.CHORUS 2%bracket_close%" --target images/10_MIDISendVoice_3.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Send Voice" --line2 "02%bracket_open%F.CHORUS 2%bracket_close%" --target images/10_MIDISendVoice_4.jpg

${LCDIMG} --line1 "OP Enable    %small_1_inv%%key%" --line2 "%small_1_inv%%small_2%%small_3_inv%%small_4_inv%%small_5_inv%%small_6_inv%" --target images/10_Operator_1.jpg
composite -gravity NorthWest -geometry "+200+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_Operator_1.jpg images/10_Operator_1.jpg
${LCDIMG} --baseimg Preset_Setting --line1 "OP Enable    %small_1_inv%%key%" --line2 "%small_1_inv%%small_2%%small_3_inv%%small_4_inv%%small_5_inv%%small_6_inv%" --target images/10_Operator_2.jpg
composite -gravity NorthWest -geometry "+200+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_Operator_2.jpg images/10_Operator_2.jpg
${LCDIMG} --baseimg Preset_Push --line1 "OP Enable    %small_1_inv%%key%" --line2 "%small_1_inv%%small_2%%small_3_inv%%small_4_inv%%small_5_inv%%small_6_inv%" --target images/10_Operator_3.jpg
composite -gravity NorthWest -geometry "+313+87" "../../images/Arrow_horizontal_blue_left_transparent.png" images/10_Operator_3.jpg images/10_Operator_3.jpg
${LCDIMG} --line1 "OP Enable    %small_1_inv%%key%" --line2 "%small_1_inv%%small_2_inv%%small_3_inv%%small_4_inv%%small_5_inv%%small_6_inv%" --target images/10_Operator_4.jpg

${LCDIMG} --baseimg Preset_Setting --line1 "Save to Bank" --line2 "76%bracket_open%STEPH2    %bracket_close%" --target images/10_SaveVoice_1.jpg
${LCDIMG} --baseimg Preset_Push --line1 "Save to Bank" --line2 "76%bracket_open%STEPH2    %bracket_close%" --target images/10_SaveVoice_2.jpg
${LCDIMG} --baseimg Preset_Setting --line1 "Save to Bank 76" --line2 "02%bracket_open%F.CHORUS 2%bracket_close%" --target images/10_SaveVoice_3.jpg
${LCDIMG} --baseimg Preset_Push --line1 "Save to Bank 76" --line2 "02%bracket_open%F.CHORUS 2%bracket_close%" --target images/10_SaveVoice_4.jpg
${LCDIMG} --baseimg Preset_Setting --line1 "Overwrite%question_mark%" --line2 "%bracket_open%NO %bracket_close%" --target images/10_SaveVoice_5.jpg
${LCDIMG} --baseimg Preset_Push --line1 "Overwrite%question_mark%" --line2 "%bracket_open%YES%bracket_close%" --target images/10_SaveVoice_6.jpg
${LCDIMG} --baseimg Volume_Push --line1 "Overwrite%question_mark%" --line2 "Canceled" --target images/10_SaveVoice_7.jpg

${LCDIMG} --baseimg Preset_Setting --line1 "MIDI Recv Bank" --line2 "76%bracket_open%STEPH2    %bracket_close%" --target images/10_BankUpload_1.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Recv Bank" --line2 "73%bracket_open%BELL04    %bracket_close%" --target images/10_BankUpload_2.jpg
${LCDIMG} --baseimg Preset_Setting --line1 "MIDI Recv Bank" --line2 "Overwrite%colon% %bracket_open%NO %bracket_close%" --target images/10_BankUpload_3.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Recv Bank" --line2 "Overwrite%colon% %bracket_open%YES%bracket_close%" --target images/10_BankUpload_4.jpg
${LCDIMG} --baseimg Preset_Setting --line1 "MIDI Recv Bank" --line2 "%bracket_open%%B_inv%ELL94    %bracket_close%" --target images/10_BankUpload_5.jpg
composite -gravity NorthWest -geometry "+232+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_BankUpload_5.jpg images/10_BankUpload_5.jpg
${LCDIMG} --line1 "MIDI Recv Bank" --line2 "%bracket_open%BELL9%4_inv%    %bracket_close%" --target images/10_BankUpload_6.jpg
composite -gravity NorthWest -geometry "+380+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_BankUpload_6.jpg images/10_BankUpload_6.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Recv Bank" --line2 "%bracket_open%BELL9%4_inv%    %bracket_close%" --target images/10_BankUpload_7.jpg
composite -gravity NorthWest -geometry "+380+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_BankUpload_7.jpg images/10_BankUpload_7.jpg
${LCDIMG} --line1 "MIDI Recv Bank" --line2 "%bracket_open%BELL9%4_inv%    %bracket_close%%sign-asterisk%" --target images/10_BankUpload_8.jpg
composite -gravity NorthWest -geometry "+380+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_BankUpload_8.jpg images/10_BankUpload_8.jpg
${LCDIMG} --baseimg Preset_Setting --line1 "MIDI Recv Bank" --line2 "%bracket_open%BELL9%5_inv%    %bracket_close%%sign-asterisk%" --target images/10_BankUpload_9.jpg
composite -gravity NorthWest -geometry "+380+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_BankUpload_9.jpg images/10_BankUpload_9.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Recv Bank" --line2 "%bracket_open%BELL9%5_inv%    %bracket_close%%sign-asterisk%" --target images/10_BankUpload_10.jpg
composite -gravity NorthWest -geometry "+380+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_BankUpload_10.jpg images/10_BankUpload_10.jpg
${LCDIMG} --line1 "MIDI Recv Bank" --line2 "%bracket_open%BELL9%5_inv%    %bracket_close%" --target images/10_BankUpload_11.jpg
composite -gravity NorthWest -geometry "+380+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_BankUpload_11.jpg images/10_BankUpload_11.jpg
${LCDIMG} --baseimg Preset_Right --line1 "MIDI Recv Bank" --line2 "%bracket_open%BELL95 %block%    %bracket_close%" --target images/10_BankUpload_12.jpg
composite -gravity NorthWest -geometry "+440+87" "../../images/Arrow_horizontal_blue_right_transparent.png" images/10_BankUpload_12.jpg images/10_BankUpload_12.jpg
${LCDIMG} --line1 "MIDI Recv Bank" --line2 " BELL95     %bracket_open%OK%bracket_close%" --target images/10_BankUpload_13.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Recv Bank" --line2 " BELL95     %bracket_open%OK%bracket_close%" --target images/10_BankUpload_14.jpg
${LCDIMG} --line1 "MIDI Recv Bank" --line2 "Waiting..." --target images/10_BankUpload_15.jpg
${LCDIMG} --baseimg Volume_Push --line1 "MIDI Recv Bank" --line2 "Canceled" --target images/10_BankUpload_16.jpg

${LCDIMG} --baseimg Preset_Setting --line1 "MIDI Send Bank" --line2 "54%bracket_open%BRASS3    %bracket_close%" --target images/10_BankSend_1.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Send Bank" --line2 "Sending Ch. 4" --target images/10_BankSend_2.jpg

${LCDIMG} --baseimg Preset_Setting --line1 "MIDI Send Voice" --line2 "65%bracket_open%CHOIR01   %bracket_close%" --target images/10_VoiceSend_1.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Send Voice" --line2 "65%bracket_open%CHOIR01   %bracket_close%" --target images/10_VoiceSend_2.jpg
${LCDIMG} --baseimg Preset_Setting --line1 "MIDI Send Voice" --line2 "06%bracket_open%CH0R 3    A%bracket_close%" --target images/10_VoiceSend_3.jpg
${LCDIMG} --baseimg Preset_Push --line1 "MIDI Send Voice" --line2 "Sending Ch. 16" --target images/10_VoiceSend_4.jpg

${LCDIMG} --line1 "1.0.0%sign-minus%3.6FX%sign-minus%16" --line2 "SD2  FAT32 29GB" --target images/10_Info_1.jpg
