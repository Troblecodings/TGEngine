#include "LoadFont.hpp"



void loadfont(char* path) {
	

/*
	std::ifstream stream(path, std::ios::ate | std::ios::binary);

	if (!stream.is_open()) {
		std::cout << "Can't open file: " << path << std::endl;
		return;
	}

	size_t filesize = stream.tellg();
	stream.seekg(0);

	FontTableEntry fnt_table_entry = {};

	uint32_t current = -1;
	char buffer[4];
	FontTable table_enum = FontTable::NaN;
	char id = 0;
	for (size_t i = 0; i < filesize; i++)
	{
		char chr;
		stream.read(&chr, 1);
		SHIFT_ADD(chr, buffer)
		if (id == -1)id = chr;
		char* last_enum;
		FontTable l_enum;
		for (int x = 0; x < 45; x++)
		{  
			l_enum = static_cast<FontTable>(x);
			last_enum = getName(l_enum);
			if (strncmp(buffer, last_enum, l_enum == FontTable::cvt ? 3:4) == 0) {

			}
		}
	}
*/
}

uint32_t FontTableEntry::createIfNotExists(char* nm)
{
	uint32_t index = 0;

	OUT_LV_DEBUG("New Table entry:")
	OUT_LV_DEBUG(nm)

	for (char* name : this->name_entry) {
		if (strcmp(name, nm) == 0)return index;
		index++;
	}

	index = this->name_entry.size();
	this->name_entry.resize(index + 1);
	this->name_entry[index] = nm;
	this->content.resize(index + 1);

	return index;
}

char* getName(FontTable table) {

	switch (table)
	{
	case FontTable::acnt:return "acnt";
	case FontTable::ankr:return "ankr";
	case FontTable::avar:return "avar";
	case FontTable::bdat:return "bdat";
	case FontTable::bhed:return "bhed";
	case FontTable::bloc:return "bloc";
	case FontTable::bsln:return "bsln";
	case FontTable::cmap:return "cmap";
	case FontTable::cvar:return "cvar";
	case FontTable::cvt: return "cvt,";
	case FontTable::EBSC:return "EBSC";
	case FontTable::fdsc:return "fdsc";
	case FontTable::feat:return "feat";
	case FontTable::fmtx:return "fmtx";
	case FontTable::fond:return "fond";
	case FontTable::fpgm:return "fpgm";
	case FontTable::fvar:return "fvar";
	case FontTable::gasp:return "gasp";
	case FontTable::glyf:return "glyf";
	case FontTable::gvar:return "gvar";
	case FontTable::hdmx:return "hdmx";
	case FontTable::head:return "head";
	case FontTable::hhea:return "hhea";
	case FontTable::hmtx:return "hmtx";
	case FontTable::just:return "just";
	case FontTable::kern:return "kern";
	case FontTable::kerx:return "kerx";
	case FontTable::lcar:return "lcar";
	case FontTable::loca:return "loca";
	case FontTable::ltag:return "ltag";
	case FontTable::maxp:return "maxp";
	case FontTable::meta:return "meta";
	case FontTable::mort:return "mort";
	case FontTable::morx:return "morx";
	case FontTable::name:return "name";
	case FontTable::opbd:return "opbd";
	case FontTable::OSSLASH2 : return "OS/2";
	case FontTable::prep:return "prep";
	case FontTable::prop:return "prop";
	case FontTable::sbix:return "sbix";
	case FontTable::trak:return "trak";
	case FontTable::vhea:return "vhea";
	case FontTable::vmtx:return "vmtx";
	case FontTable::xref:return	"xref";
	case FontTable::Zapf:return "Zapf";
	}
	return nullptr;
}