// this file is distributed under 
// MIT license
#include <HV/HVSetter.h>
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
namespace HVAdjust{
	HVTable::HVTable(const HVconfig&config,const Setup& setup,const Frame& frame,const shared_ptr<IDataSource>src)
	:f_config(config),f_setup(setup),f_frame(frame),f_pmhv_conn(src),f_photomultipliers(src){update();}
	HVTable::~HVTable(){}
	void HVTable::update(){
		f_items.clear();
		auto f_entries_cache=f_config.CreateEntriesFactory().GetList();
		for(const Layer& layer:f_frame.CreateLayersFactory().GetList())
			for(const Slot&slot:layer.CreateSlotsFactory().GetList())
				for(const HVPMConnection&conn:f_pmhv_conn.BySlotID(slot.id()))
					if(conn.setup_id()==f_setup.id()){
						HVconfigEntry hventry(conn.id(),0.0);
						for(const HVconfigEntry&item:f_entries_cache)
							if(item.HVPMConnection_id()==conn.id())
								hventry=item;
						f_items.push_back({
							.layer=layer,.slot=slot,.hvpm=conn,
							.phm=f_photomultipliers.ByID(conn.photomultiplier_id()),
							.entry=hventry
						});
					}
	}
	const size_t HVTable::size()const{return f_items.size();}
	HVTable::const_iterator HVTable::begin() const{return f_items.begin();}
	HVTable::const_iterator HVTable::cbegin() const{return f_items.cbegin();}
	HVTable::const_iterator HVTable::end() const{return f_items.end();}
	HVTable::const_iterator HVTable::cend() const{return f_items.cend();}
	const HVTable::Item& HVTable::operator[](const size_t index)const{return f_items[index];}
	bool HVTable::SetHV(const size_t index, const double hv){
		if(index>=size())return false;
		if(hv*f_items[index].phm.max_hv()<0.0)return false;
		if(hv/f_items[index].phm.max_hv()>1.0)return false;
		vector<HVconfigEntry> tmp;
		auto entries=f_config.CreateEntriesFactory();
		for(const HVconfigEntry&entry:entries.GetList())
			if(entry.HVPMConnection_id()==f_items[index].hvpm.id())
				tmp.push_back(entry);
		for(auto&item:tmp)
			if(!entries.Delete(item))
				return false;
		auto res=entries.Add(HVconfigEntry(f_items[index].hvpm.id(),hv));
		update();
		return res;
	}
}