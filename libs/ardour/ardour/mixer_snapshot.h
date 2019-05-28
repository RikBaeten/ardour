/*
    Copyright (C) 2019 Nikolaus Gullotta

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __ardour_mixer_snapshot_h__
#define __ardour_mixer_snapshot_h__

#include <vector>
#include <ctime>

#include "ardour/session.h"
#include "ardour/route.h"
#include "ardour/vca.h"
#include "ardour/route_group.h"

namespace ARDOUR {

class LIBARDOUR_API MixerSnapshot
{
    public:
        enum RecallFlags {
            RecallEQ = 0x1,
            RecallComp = 0x2,
            RecallIO = 0x4,
            RecallGroup = 0x8,
            RecallVCA = 0x10,
        };

        MixerSnapshot(ARDOUR::Session*);
        MixerSnapshot(ARDOUR::Session*, std::string);
        ~MixerSnapshot();

        void snap();
        void snap(ARDOUR::RouteList);
        void snap(ARDOUR::RouteGroup*);
        void snap(boost::shared_ptr<ARDOUR::VCA>);
        void snap(boost::shared_ptr<ARDOUR::Route>);
        void recall();
        void clear();
        void write(const std::string);
        void load(const std::string);
        bool has_specials();

        ARDOUR::Session* get_session() {return _session;};

        struct State {
            std::string id;
            std::string name;
            XMLNode     node;
        };

        bool empty() {
            return (
                route_states.empty() &&
                group_states.empty() &&
                vca_states.empty()
            );
        };

        std::vector<State> get_routes() {return route_states;};
        std::vector<State> get_groups() {return group_states;};
        std::vector<State> get_vcas()   {return vca_states;};
#ifdef MIXBUS
        bool get_recall_eq()    const { return _flags & RecallEQ;};
        bool get_recall_comp()  const { return _flags & RecallComp;};
#endif
        bool get_recall_io()    const { return _flags & RecallIO;};
        bool get_recall_group() const { return _flags & RecallGroup;};
        bool get_recall_vca()   const { return _flags & RecallVCA;};

#ifdef MIXBUS
        void set_recall_eq(bool);
        void set_recall_comp(bool);
#endif
        void set_recall_io(bool);
        void set_recall_group(bool);
        void set_recall_vca(bool);

        unsigned int get_id() {return id;};
        void set_id(unsigned int new_id) {id = new_id;};

        std::string get_label() {return label;};
        void set_label(std::string new_label) {label = new_label;};

        bool get_favorite() {return favorite;};
        void set_favorite(bool yn) {favorite = yn;};

        std::time_t get_timestamp() {return timestamp;};
        void set_timestamp(std::time_t new_timestamp) {timestamp = new_timestamp;};

        std::string get_last_modified_with() {return last_modified_with;};
        void set_last_modified_with(std::string new_modified_with) {last_modified_with = new_modified_with;};

        void set_route_states(std::vector<State> states) { route_states = states;};

    private:
        ARDOUR::Session* _session;

        XMLNode& sanitize_node(XMLNode&);
        void reassign_masters(boost::shared_ptr<ARDOUR::Slavable>, XMLNode);
        void load_from_session(std::string);
        void load_from_session(XMLNode&);
        bool set_flag(bool, RecallFlags);

        unsigned int id;
        bool favorite;
        std::string label;
        std::time_t timestamp;
        std::string last_modified_with;
        RecallFlags _flags;

        std::vector<State> route_states;
        std::vector<State> group_states;
        std::vector<State> vca_states;

};

} // namespace ARDOUR

#endif /* __ardour_mixer_snapshot_h__ */