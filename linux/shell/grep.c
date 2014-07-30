
三、grep
	globe regular expression print

	man grep查看grep synopsis.


	grep "r..t" passwd              搜索第一个字符是r，第二和第三个字符为任意字符，第四字符为t的字符串的行
	grep "r.*t" passwd              搜索第一个字符是r，中间为任意长度任意字符，最后一个字符为t的字符串的行
	grep "r*t"  passwd              搜索第一个是零个r或多个r,最后一个字符为t的字符串的行
	grep "^r" passwd                搜索以r为首字符的行
	grep "t$" passwd                搜索以t为尾字符的行
	grep "^rt$" passwd              搜索只有两人字符为rt的行
	grep "\<root" passwd            搜索以root开头的单词的行
	grep "root\>" passwd            搜索以root结尾的单词的行
	grep "\<root\>" passwd          搜索完全匹配root的单词的行
	grep "[abc]" passwd             搜索有a或b或c字符的行
	grep "[^abc]" passwd            搜索没有a或b或c字符的行
	grep "ga\{3\}" passwd           搜索以g开头，后面跟3个a的字符串的行
	grep "ga\{3,\}" passwd          搜索以g开头，后面跟3个以上的a字符的字符串的行
	grep "ga\{3,6\}" passwd         搜索以g开头，后面跟3个到6个a字符的字符串的行

	egrep "r+t" passwd              搜索以一个r或多个r,后面跟t的字符串的行
	egrep "r?t" passwd              搜索以一个r或空,后面跟t的字符串的行
	egrep "r|t" passwd              搜索出现r或t字符的行
	egrep "root|madao" passwd         搜索出现root或madao单词的行
	egrep "r(abc)+t" passwd         以r开头中间至少有一个或多个abc后面跟上t字符的字符串的行

============================================================================

dialog

--checklist	text height width list-height [tag text status]
--infobox	text height width
--inputbox	text height width [inital string]
--menu		text height width menu-height [tag item]...
--msgbox	text height width
--radiolist	text height width list-height [tag txt status]...
--textbox	filename height width
--yesno		text height width

dialog --title "Check me" --checklist "Pick Numbers" 15 36 3 1 "one" "off" 2 "two" "on" 3 "three" "off"


#!/bin/bash

# 1. 用一个消息框提示用户
dialog --title "Questionnaire" --msgbox "Welcome to my simple survey" 9 48

# 2. 用一个yesno框询问用户是否要继续
dialog --title "Confirm" --yesno "Are you willing to take part?" 9 48
if [ $? != 0 ]; then
	dialog --infobox "Thank you anyway" 5 36
	sleep 2
	dialog --clear
	exit 0
fi

# 3. 用一个输入框询问用户姓名. 重定向标准错误流2到临时文件/tmp/_1dialog.txt
# 然后再将其放到变量Q_NAME中.
dialog --title "Questionnaire" --inputbox "Pls enter your name:" 9 48 2>/tmp/_1dialog.txt
Q_NAME=$(cat /tmp/_1dialog.txt)

# 4. 显示一个菜单, 再次重定向标准错误流并将其装载到一个变量中.
dialog --menu "$Q_NAME, what music do you like best?" 15 48 4 1 "Classical" 2 "Jazz" 3 "Country" 4 "Other" 2>/tmp/_1dialog.txt
Q_MUSIC=$(cat /tmp/_1dialog.txt)
if [ "$Q_MUSIC" = "1" ]; then
	dialog --title "Likes Classical" --msgbox "Good choice!" 12 36
else
	dialog --title "Doesn't like Classical" --msgbox "Shame" 12 36
fi

sleep 1
clear
exit 0

