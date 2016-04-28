// this file is distributed under 
// MIT license
#include <JPetData/Frames.h>
using namespace std;
using namespace DataAccess;
namespace JPetSetup {
	Slot::Slot(const Slot& source)
	:f_id(source.f_id),f_layer_id(source.f_layer_id),f_name(source.f_name),f_active(source.f_active){}
	Slot::Slot(const DataItem& item, const shared_ptr<IDataSource>)
	:f_id(item.num_field<size_t>("id")),f_layer_id(item.num_field<size_t>("layer_id")),
	f_name(item["name"]),f_active(item.flag("isactive")){}
	Slot::~Slot(){}
	const size_t Slot::id() const{return f_id;}
	const string& Slot::name() const{return f_name;}
	const bool Slot::active() const{return f_active;}
	const size_t Slot::layer_id() const{return f_layer_id;}
	
	Layer::Layer(const Layer& source)
	:f_id(source.f_id),f_frame_id(source.f_frame_id),f_name(source.f_name),
	f_radius(source.f_radius),f_active(source.f_active),f_data_source(source.f_data_source){}
	Layer::Layer(const DataItem& item, const shared_ptr< IDataSource > src)
	:f_id(item.num_field<size_t>("id")),f_frame_id(item.num_field<size_t>("frame_id")),f_name(item["name"]),
	f_radius(item.num_field<double>("radius")),f_active(item.flag("isactive")),f_data_source(src){}
	Layer::~Layer(){}
	const size_t Layer::id() const{return f_id;}
	const size_t Layer::frame_id() const{return f_frame_id;}
	const string& Layer::name() const{return f_name;}
	const double Layer::radius() const{return f_radius;}
	const bool Layer::active() const{return f_active;}
	Factory<Slot> Layer::CreateSlotsFactory() const{return Factory<Slot>(f_data_source,{to_string(id())});}

	Frame::Frame(const Frame& source)
	:f_id(source.f_id),f_version(source.f_version),
	f_description(source.f_description),f_status(source.f_status),
	f_active(source.f_active),f_data_source(source.f_data_source){}
	Frame::Frame(const DataItem& item, const shared_ptr< IDataSource > src)
	:f_id(item.num_field<size_t>("id")),f_version(item.num_field<size_t>("version")),
	f_description(item["description"]),f_status(item["status"]),
	f_active(item.flag("isactive")),f_data_source(src){}
	Frame::~Frame(){}
	const size_t Frame::id() const{return f_id;}
	const string& Frame::description() const{return f_description;}
	const size_t Frame::version() const{return f_version;}
	const string& Frame::status() const{return f_status;}
	const bool Frame::active() const{return f_active;}
	Factory<Layer> Frame::CreateLayersFactory() const{return Factory<Layer>(f_data_source,{to_string(id())});}
	
	Frames::Frames(const shared_ptr< IDataSource > src):Factory<Frame>(src,{}){}
	Frames::~Frames(){}
};