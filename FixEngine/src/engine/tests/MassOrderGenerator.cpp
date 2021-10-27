#include "engine/tests/MassOrderGenerator.h"

namespace RLMTest
{
	MassOrderGenerator::MassOrderGenerator(Session *sn)
	{
		srand ((unsigned int)time(NULL));
		this->sn = sn;

        MassQuote = new Message("i", ProtocolVersion::FIX_44);

        MassQuote->set(Tags::SenderCompID,sn->senderCompId());
        MassQuote->set(Tags::TargetCompID,sn->targetCompId());
        MassQuote->set(Tags::SendingTime, Timestamp::utc(), TimestampFormat::YYYYMMDDHHMMSSNsec);
        MassQuote->set(Tags::QuoteReqID,"33000000009");
        MassQuote->set(Tags::QuoteID,1);
        MassQuote->set(Tags::NoQuoteSets,1);

        Group groupQuoteSets = MassQuote->setGroup(Tags::NoQuoteSets, 1);
        groupQuoteSets[0].set(Tags::QuoteSetID, "QuoteSetID");
        groupQuoteSets[0].set(Tags::TotNoQuoteEntries,1);

        float BidPx = ((float)rand()/(float)(RAND_MAX));

        QuoteEntryIDs = groupQuoteSets[0].setGroup(Tags::NoQuoteEntries, 1);
        QuoteEntryIDs[0].set(Tags::QuoteEntryID,1);
        QuoteEntryIDs[0].set(Tags::Symbol, "GBP/USD");
        QuoteEntryIDs[0].set(Tags::BidPx,BidPx);
        QuoteEntryIDs[0].set(Tags::OfferPx,BidPx*1.01);
        QuoteEntryIDs[0].set(Tags::BidSize,100000);
        QuoteEntryIDs[0].set(Tags::OfferSize,100000);
        QuoteEntryIDs[0].set(Tags::SettlDate,Timestamp::utc(), TimestampFormat::YYYYMMDD);
	}

	MassOrderGenerator::~MassOrderGenerator()
	{
	}

    OnixS::FIX::Message * MassOrderGenerator::GenerateMessage(int i)
	{
        MassQuote->set(Tags::SendingTime, Timestamp::utc(), TimestampFormat::YYYYMMDDHHMMSSNsec);
        MassQuote->set(Tags::QuoteID,i);

		float BidPx = ((float)rand()/(float)(RAND_MAX));

		QuoteEntryIDs[0].set(Tags::BidPx,BidPx);
		QuoteEntryIDs[0].set(Tags::OfferPx,BidPx*1.01);
		QuoteEntryIDs[0].set(Tags::SettlDate,Timestamp::utc(), TimestampFormat::YYYYMMDD);

        return MassQuote;
	}
}
