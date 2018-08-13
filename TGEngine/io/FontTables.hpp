#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "..\Util\Annotations.hpp"

enum struct FontTable {
	acnt,
	ankr,
	avar,
	bdat,
	bhed,
	bloc,
	bsln,
	cmap,
	cvar,
	cvt,
	EBSC,
	fdsc,
	feat,
	fmtx,
	fond,
	fpgm,
	fvar,
	gasp,
	glyf,
	gvar,
	hdmx,
	head,
	hhea,
	hmtx,
	just,
	kern,
	kerx,
	lcar,
	loca,
	ltag,
	maxp,
	meta,
	mort,
	morx,
	name,
	opbd,
	OSSLASH2,
	prep,
	prop,
	sbix,
	trak,
	vhea,
	vmtx,
	xref,
	Zapf,
	NaN = -1
};

struct BaseTable {
	FontTable table_id = FontTable::NaN;

	virtual void extractDataToStruct(std::ifstream str) {};
};

SINCE(0, 0, 2)
DEPRECATED
UNFINISHED
struct acntTable : BaseTable {

	//Header
	uint32_t version;
	uint16_t firstAccentGlyphIndex;
	uint16_t lastAccentGlyphIndex;
	uint32_t descritptionOffset;
	uint32_t extensionOffset;
	uint32_t secondaryOffset;
	uint64_t glyphs;
	uint64_t ext;
	uint32_t accents;

	//Description
	bool description;
	uint16_t primarGlyphIndex;

	//Format 0
	uint8_t primaryAttechmentPoint;
	uint8_t secondaryInfoIndex;

	//Format 1
	uint16_t extensionsOffset;

	//Extension
	bool components;
	std::vector<uint8_t> secondaryInfoIdicies;
	std::vector<uint8_t> primaryAttachmentPoint;

	//Secondary data
	uint16_t secondaryGlyphIndex;
	uint8_t secondaryGlyphAttachmentNumber;

	acntTable() {
		table_id = FontTable::acnt;
	}

	void extractDataToStruct(std::ifstream str);
};

SINCE(0, 0, 2)
UNFINISHED
struct ankrTable : BaseTable {

	//Header
	uint16_t version;
	uint16_t flags;
	uint32_t lookuptable_offset;
	uint32_t glyphDataTableOffset;
	
	// Glyphdata Table
	uint32_t numPoints;
	std::vector<uint32_t> anchorPoints;

	ankrTable() {
		table_id = FontTable::ankr;
	}

	void extractDataToStruct(std::ifstream str);
};

struct avarTable : BaseTable {
	avarTable() {
		table_id = FontTable::avar;
	}

	void extractDataToStruct(std::ifstream str);

};
struct bdatTable : BaseTable {
	bdatTable() {
		table_id = FontTable::bdat;
	}

	void extractDataToStruct(std::ifstream str);

};
struct bhedTable : BaseTable { 
	bhedTable() {
		table_id = FontTable::bhed;
	}

	void extractDataToStruct(std::ifstream str);

};
struct blocTable : BaseTable {
	blocTable() {
		table_id = FontTable::bloc;
	}

	void extractDataToStruct(std::ifstream str);

};
struct bslnTable : BaseTable { 
	bslnTable() {
		table_id = FontTable::bsln;
	}

	void extractDataToStruct(std::ifstream str);

};
struct cmapTable : BaseTable { 
	cmapTable() {
		table_id = FontTable::cmap;
	}

	void extractDataToStruct(std::ifstream str);

};
struct cvarTable : BaseTable {
	cvarTable() {
		table_id = FontTable::cvar;
	}

	void extractDataToStruct(std::ifstream str);

};
struct cvtTable : BaseTable { 
	cvtTable() {
		table_id = FontTable::cvt;
	}

	void extractDataToStruct(std::ifstream str);

};
struct EBSCTable : BaseTable {
	EBSCTable() {
		table_id = FontTable::EBSC;
	}

	void extractDataToStruct(std::ifstream str);

};
struct fdscTable : BaseTable { 
	fdscTable() {
		table_id = FontTable::fdsc;
	}

	void extractDataToStruct(std::ifstream str);
};
struct featTable : BaseTable { 
	featTable() {
		table_id = FontTable::feat;
	}

	void extractDataToStruct(std::ifstream str);

};
struct fmtxTable : BaseTable {
	fmtxTable() {
		table_id = FontTable::fmtx;
	}

	void extractDataToStruct(std::ifstream str);

};
struct fondTable : BaseTable { 
	fondTable() {
		table_id = FontTable::fond;
	}

	void extractDataToStruct(std::ifstream str);

};
struct fpgmTable : BaseTable {
	fpgmTable() {
		table_id = FontTable::fpgm;
	}

	void extractDataToStruct(std::ifstream str);

};
struct fvarTable : BaseTable {
	fvarTable() {
		table_id = FontTable::fvar;
	}

	void extractDataToStruct(std::ifstream str);

};
struct gaspTable : BaseTable { 
	gaspTable() {
		table_id = FontTable::gasp;
	}

	void extractDataToStruct(std::ifstream str);

};
struct glyfTable : BaseTable { 
	glyfTable() {
		table_id = FontTable::glyf;
	}

	void extractDataToStruct(std::ifstream str);

};
struct gvarTable : BaseTable { 
	gvarTable() {
		table_id = FontTable::gvar;
	}

	void extractDataToStruct(std::ifstream str);

};
struct hdmxTable : BaseTable { 
	hdmxTable() {
		table_id = FontTable::hdmx;
	}

	void extractDataToStruct(std::ifstream str);

};
struct headTable : BaseTable {
	headTable() {
		table_id = FontTable::head;
	}

	void extractDataToStruct(std::ifstream str);

};
struct hheaTable : BaseTable {
	hheaTable() {
		table_id = FontTable::hhea;
	}

	void extractDataToStruct(std::ifstream str);

};
struct hmtxTable : BaseTable {
	hmtxTable() {
		table_id = FontTable::hmtx;
	}

	void extractDataToStruct(std::ifstream str);

};
struct justTable : BaseTable { 
	justTable() {
		table_id = FontTable::just;
	}

	void extractDataToStruct(std::ifstream str);

};
struct kernTable : BaseTable {
	kernTable() {
		table_id = FontTable::kern;
	}

	void extractDataToStruct(std::ifstream str);

};
struct kerxTable : BaseTable {
	kerxTable() {
		table_id = FontTable::kerx;
	}

	void extractDataToStruct(std::ifstream str);

};
struct lcarTable : BaseTable {
	lcarTable() {
		table_id = FontTable::lcar;
	}

	void extractDataToStruct(std::ifstream str);

};
struct locaTable : BaseTable {
	locaTable() {
		table_id = FontTable::loca;
	}

	void extractDataToStruct(std::ifstream str);

};
struct ltagTable : BaseTable {
	ltagTable() {
		table_id = FontTable::ltag;
	}

	void extractDataToStruct(std::ifstream str);

};
struct maxpTable : BaseTable {
	maxpTable() {
		table_id = FontTable::maxp;
	}

	void extractDataToStruct(std::ifstream str);

};
struct metaTable : BaseTable {
	metaTable() {
		table_id = FontTable::meta;
	}

	void extractDataToStruct(std::ifstream str);

};

SINCE(0, 0, 2)
DEPRECATED
struct mortTable : BaseTable {
	mortTable() {
		table_id = FontTable::mort;
	}

	void extractDataToStruct(std::ifstream str);

};
struct morxTable : BaseTable {
	morxTable() {
		table_id = FontTable::morx;
	}

	void extractDataToStruct(std::ifstream str);

};
struct nameTable : BaseTable {
	nameTable() {
		table_id = FontTable::name;
	}

	void extractDataToStruct(std::ifstream str);

};
struct opbdTable : BaseTable {
	opbdTable() {
		table_id = FontTable::opbd;
	}

	void extractDataToStruct(std::ifstream str);

};
struct OSSLASH2Table : BaseTable {
	OSSLASH2Table() {
		table_id = FontTable::OSSLASH2;
	}

	void extractDataToStruct(std::ifstream str);

};
struct prepTable : BaseTable {
	prepTable() {
		table_id = FontTable::prep;
	}

	void extractDataToStruct(std::ifstream str);

};
struct propTable : BaseTable {
	propTable() {
		table_id = FontTable::prop;
	}

	void extractDataToStruct(std::ifstream str);

};
struct sbixTable : BaseTable {
	sbixTable() {
		table_id = FontTable::sbix;
	}

	void extractDataToStruct(std::ifstream str);

};
struct trakTable : BaseTable {
	trakTable() {
		table_id = FontTable::trak;
	}

	void extractDataToStruct(std::ifstream str);

};
struct vheaTable : BaseTable {
	vheaTable() {
		table_id = FontTable::vhea;
	}

	void extractDataToStruct(std::ifstream str);

};
struct vmtxTable : BaseTable {
	vmtxTable() {
		table_id = FontTable::vmtx;
	}

	void extractDataToStruct(std::ifstream str);

};
struct xrefTable : BaseTable {
	xrefTable() {
		table_id = FontTable::xref;
	}

	void extractDataToStruct(std::ifstream str);

};
struct ZapfTable : BaseTable {
	ZapfTable() {
		table_id = FontTable::Zapf;
	}

	void extractDataToStruct(std::ifstream str);

};
