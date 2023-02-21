#include "DataManager.h"
#include "DataItemEditWidget.h"
#include "DataUser.h"
#include <cstdarg>
#include <QGroupBox>

DataManager::DataManager(){
}

DataManager::~DataManager(){
}

DataManager* DataManager::sharedManager(){
	static DataManager shared_manager;
	return &shared_manager;
}

bool DataManager::createInt(std::string name, const int& value,
							const int& maxv, const int& minv, DataUser* user,
							const bool& bSliderUpdate)
{
	bool bNonExisting = true;
	if (integers.find(name)!=integers.end()){
		printf("Err: DataManager: Fail to create item. %s\n", name.c_str());
		bNonExisting = false;
	}

	integers[name].current = value;
	integers[name].maxv = maxv;
	integers[name].minv = minv;
	integers[name].bSliderUpdate = bSliderUpdate;
	if(user!=NULL) dataUsers[name].insert(user);

	return bNonExisting;
}

bool DataManager::createFloat(std::string name, const float& value,
							  const float& maxv, const float& minv, DataUser* user,
							  const bool& bSliderUpdate)
{
	bool bNonExisting = true;
	if (floats.find(name)!=floats.end()){
		printf("Err: DataManager: Fail to create item. %s\n", name.c_str());
		bNonExisting = false;
	}

	floats[name].current = value;
	floats[name].maxv = maxv;
	floats[name].minv = minv;
	floats[name].bSliderUpdate = bSliderUpdate;
	if(user!=NULL) dataUsers[name].insert(user);

	return bNonExisting;
}

bool DataManager::createShort(std::string name, const short& value,
							  const short& maxv, const short& minv, DataUser* user,
							  const bool& bSliderUpdate)
{
	bool bNonExisting = true;
	if (shorts.find(name)!=shorts.end()){
		printf("Err: DataManager: Fail to create item. %s\n", name.c_str());
		bNonExisting = false;
	}

	shorts[name].current = value;
	shorts[name].maxv = maxv;
	shorts[name].minv = minv;
	shorts[name].bSliderUpdate = bSliderUpdate;
	if(user!=NULL) dataUsers[name].insert(user);

	return bNonExisting;
}

bool DataManager::createBool(std::string name, const bool& value, DataUser* user){
	bool bNonExisting = true;
	if (bools.find(name)!=bools.end()){
		printf("Err: DataManager: Fail to create item. %s\n", name.c_str());
		bNonExisting = false;
	}

	bools[name] = value;
	if(user!=NULL) dataUsers[name].insert(user);

	return bNonExisting;
}

bool DataManager::createColor( std::string name, const vec4f& value, DataUser* user){
	bool bNonExisting = true;
	if (colors.find(name)!=colors.end()){
		printf("Err: DataManager: Fail to create item. %s\n", name.c_str());
		bNonExisting = false;
	}

	colors[name] = value;
	if(user!=NULL) dataUsers[name].insert(user);

	return bNonExisting;
}

bool DataManager::createEnum(std::string name, const std::vector<std::string>& val_names, 
							 const int& value, const DataItemEnumWidgetType& widget_type, DataUser* user){
	bool bNonExisting = true;
	if (enums.find(name)!=enums.end()){
		printf("Err: DataManager: Fail to create item. %s\n", name.c_str());
		bNonExisting = false;
	}

	DataItemEnum enum_item;
	enum_item.val = value;
	enum_item.names.assign(val_names.begin(), val_names.end());
	enum_item.widget_type = (DataItemEnumWidgetType)clamp((int)widget_type, 0, 1);
	enums[name] = enum_item;
	if(user!=NULL) dataUsers[name].insert(user);

	return bNonExisting;
}

bool DataManager::createPointer(std::string name, const PointerType& value, DataUser* user){
	bool bNonExisting = true;
	if (pointers.find(name)!=pointers.end()){
		printf("Err: DataManager: Fail to create item. %s\n", name.c_str());
		bNonExisting = false;
	}

	pointers[name] = value;
	if(user!=NULL) dataUsers[name].insert(user);

	return bNonExisting;
}

bool DataManager::createTrigger(std::string name, DataUser* user){
	bool bNonExisting = true;
	if (dataUsers.find(name)!=dataUsers.end()){
		printf("Err: DataManager: Fail to create item. %s\n", name.c_str());
		bNonExisting = false;
	}

	if(user!=NULL) dataUsers[name].insert(user);

	return bNonExisting;
}

bool DataManager::registerItem(const std::string& name, DataUser* user){
	if (dataUsers.find(name)==dataUsers.end()){
		printf("Err: DataManager: Fail to register item.\n");
		return false;
	}

	dataUsers[name].insert(user);

	return true;
}

int DataManager::getIntValue(const std::string& name, bool& bSuccess){
	if (integers.find(name)==integers.end()){
		bSuccess = false;
		printf("Err: Fail to get value:%s.\n", name.c_str());
		return 0;
	}
	bSuccess = true;
	return (integers[name].current);
}

float DataManager::getFloatValue(const std::string& name, bool& bSuccess){
	if (floats.find(name)==floats.end()){
		bSuccess = false;
		printf("Err: Fail to get value:%s.\n", name.c_str());
		return 0;
	}
	bSuccess = true;
	return (floats[name].current);
}

short DataManager::getShortValue(const std::string& name, bool& bSuccess){
	if (shorts.find(name)==shorts.end()){
		bSuccess = false;
		printf("Err: Fail to get value:%s.\n", name.c_str());
		return 0;
	}
	bSuccess = true;
	return (shorts[name].current);
}

bool DataManager::getBoolValue(const std::string& name, bool& bSuccess){
	if (bools.find(name)==bools.end()){
		bSuccess = false;
		printf("Err: Fail to get value:%s.\n", name.c_str());
		return 0;
	}
	bSuccess = true;
	return (bools[name]);
}

vec4f DataManager::getColorValue(const std::string& name, bool& bSuccess){
	if (colors.find(name)==colors.end()){
		bSuccess = false;
		printf("Err: Fail to get value:%s.\n", name.c_str());
		return makeVec4f(0,0,0,0);
	}
	bSuccess = true;
	return (colors[name]);
}

int DataManager::getEnumValue(const std::string& name, bool& bSuccess){
	if (enums.find(name)==enums.end()){
		bSuccess = false;
		printf("Err: Fail to get value:%s.\n", name.c_str());
		return 0;
	}
	bSuccess = true;
	return (enums[name].val);
}


PointerType DataManager::getPointerValue(const std::string& name, bool& bSuccess){
	if (pointers.find(name)==pointers.end()){
		bSuccess = false;
		printf("Err: Fail to get value:%s.\n", name.c_str());
		return 0;
	}
	bSuccess = true;
	return (pointers[name]);
}

bool DataManager::setIntValue(const std::string& name, const int& value){
	if (integers.find(name)==integers.end()){
		createInt(name, value);
		printf("Err: Fail to set value:%s.\n", name.c_str());
		return false;
	}
	integers[name].current = value;
	notifyChange(name);
	updateDisplayValue(name);
	return true;
}

bool DataManager::setFloatValue(const std::string& name, const float& value){
	if (floats.find(name)==floats.end()){
		createFloat(name, value);
		printf("Err: Fail to set value:%s.\n", name.c_str());
		return false;
	}
	floats[name].current = value;
	notifyChange(name);
	updateDisplayValue(name);
	return true;
}

bool DataManager::setShortValue(const std::string& name, const short& value){
	if (shorts.find(name)==shorts.end()){
		createShort(name, value);
		printf("Err: Fail to set value:%s.\n", name.c_str());
		return false;
	}
	shorts[name].current = value;
	notifyChange(name);
	updateDisplayValue(name);
	return true;
}

bool DataManager::setBoolValue(const std::string& name, const bool& value){
	if (bools.find(name)==bools.end()){
		createBool(name, value);
		printf("Err: Fail to set value:%s.\n", name.c_str());
		return false;
	}
	bools[name] = value;
	notifyChange(name);
	updateDisplayValue(name);
	return true;
}

bool DataManager::setColorValue( const std::string& name, const vec4f& value ){
	if (colors.find(name)==colors.end()){
		createColor(name, value);
		printf("Err: Fail to set value:%s.\n", name.c_str());
		return false;
	}
	colors[name] = value;
	notifyChange(name);
	updateDisplayValue(name);
	return true;
}

bool DataManager::setEnumValue( const std::string& name, const int& value ){
	if (enums.find(name)==enums.end()){
		//createColor(name, value);
		printf("Err: Fail to find enum item:%s.\n", name.c_str());
		return false;
	}
	if (value<0 || value>=enums[name].names.size()) {
		printf("Err: Fail to set value:%s.\n", name.c_str());
		return false;
	}
	enums[name].val = value;
	notifyChange(name);
	updateDisplayValue(name);
	return true;
}

bool DataManager::setPointerValue(const std::string& name, const PointerType& value){
	if (pointers.find(name)==pointers.end()){
		createBool(name, value);
		printf("Err: Fail to set value:%s.\n", name.c_str());
		return false;
	}
	pointers[name] = value;
	notifyChange(name);
	return true;
}

void DataManager::trigger(const std::string& name){
	notifyChange(name);
}

QWidget* DataManager::createInterface(std::string title, std::vector<std::string> names, QWidget* parent){
#if defined(__WINDOWS__) || defined (__WIN32) || defined (WIN32) || defined (__WIN32__) || defined (__WIN64)
	QFont font("MS Shell Dlg 2", 8);
	int y=5, yd=45, yd2=25, yd3=30;
#elif defined (__APPLE__)
	QFont font("Helvetica", 11);
	int y=10, yd=40, yd2=20, yd3=25;
#else
	QFont font("Times", 8);
	int y=5, yd=45, yd2=25, yd3=30;
#endif

	QWidget* win;
	QGroupBox* gb;
	if (parent==NULL) {
		win = new QWidget();
		win->setWindowTitle(QString(title.c_str()));
	} else {
		win = new QWidget(parent);
		gb = new QGroupBox(QString(title.c_str()), win);
		gb->setFlat(false);
		gb->setFont(font);
		y += 15;
	} 
	
	int num = names.size();
	for (int i=0; i<num; ++i){
		std::string name = names[i];
		DataItemEditWidget* widget = NULL;
		if(integers.find(name)!=integers.end()){
			DataItemInt& item = integers[name];
			widget = new DataItemEditWidget(name, DATA_ITEM_INT, 
				item.minv, item.maxv, item.bSliderUpdate, this, win);
			widget->setGeometry(10, y, 240, yd);
			y += yd;
		} else if(floats.find(name)!=floats.end()){
			DataItemFloat& item = floats[name];
			widget = new DataItemEditWidget(name, DATA_ITEM_FLOAT,
				item.minv, item.maxv, item.bSliderUpdate, this, win);
			widget->setGeometry(10, y, 240, yd);
			y += yd;
		} else if(shorts.find(name)!=shorts.end()){
			DataItemShort& item = shorts[name];
			widget = new DataItemEditWidget(name, DATA_ITEM_SHORT,
				item.minv, item.maxv, item.bSliderUpdate, this, win);
			widget->setGeometry(10, y, 240, yd);
			y += yd;
		} else if(bools.find(name)!=bools.end()){
			widget = new DataItemEditWidget(name, DATA_ITEM_BOOL,
				0, 1, false, this, win);
			widget->setGeometry(10, y, 240, yd2);
			y += yd2;
		} else if(colors.find(name)!=colors.end()){
			widget = new DataItemEditWidget(name, DATA_ITEM_COLOR,
				0, 1, false, this, win);
			widget->setGeometry(10, y, 240, yd3);
			y += yd3;
		} else if(enums.find(name)!=enums.end()){
			DataItemEnum& enum_item = enums[name];
			widget = new DataItemEditWidget(name, enum_item, this, win);
			int height;
			if (enum_item.widget_type==DATA_ITEM_ENUM_RADIOBUTTON) {
				height = 20*enum_item.names.size()+5;
			} else {
				height = 25;
			}
			widget->setGeometry(10, y, 240, height);
			y += height;
		} else if (dataUsers.find(name)!=dataUsers.end()) {//trigger
			widget = new DataItemEditWidget(name, this, win);
			widget->setGeometry(10, y, 240, yd2);
			y += yd2;
		}
		editors[name].insert(widget);
	}

	if (parent==NULL) {
		win->setGeometry(1265, 50, 260, y);
		win->show();
	} else {
		gb->setGeometry(5,5,250,y-5);
		win->setGeometry(0,0,260,y);
	}
	
	return win;
}

bool DataManager::notifyChange(const std::string& name){
	if (dataUsers.find(name)==dataUsers.end()){
		printf("Err: DataManager: Fail to notify change of item: %s.\n", name.c_str());
		return false;
	}

	DataUserSet& users = dataUsers[name];
	for (DataUserSet::iterator it=users.begin(); it!=users.end(); ++it){
		(*it)->onDataItemChanged(name);
	}

	return true;
}

bool DataManager::updateDisplayValue(const std::string& name) {
	auto editors_it = editors.find(name);
	if (editors_it == editors.end()) {
		printf("Err: DataManager: Fail to notify change of item: %s.\n", name.c_str());
		return false;
	}

	DataEditWidgetSet& item_editors = editors_it->second;
	for (auto it = item_editors.begin(); it != item_editors.end(); ++it) {
		(*it)->updateDisplayValue();
	}

	return true;
}

bool DataManager::removeDataItemEditWidget(const std::string& name, DataItemEditWidget* widget){
	auto editors_it = editors.find(name);
	if (editors_it == editors.end()) return false;

	editors_it->second.erase(widget);
	return true;
}