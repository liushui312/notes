
CheckBoxPreference 选中为true 取消选中为false 它的值会以boolean的形式储存在SharedPreferences中。

xml文件：

// 从资源文件中添Preferences ，选择的值将会自动保存到SharePreferences
addPreferencesFromResource(R.xml.checkbox);

setPersistent(false) [Preference];  不存到SharePreferences

CheckBoxPreference mCheckbox0 = (CheckBoxPreference) findPreference("checkbox_0");
