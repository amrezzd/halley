#pragma once

#include <halley/utils/averaging.h>

#include "stats_view.h"
#include "halley/core/api/core_api.h"
#include "halley/net/connection/ack_unreliable_connection_stats.h"
#include "halley/support/profiler.h"

namespace Halley
{
	class NetworkSession;
	class INetworkServiceStatsListener;
	class System;

	class PerformanceStatsView : public StatsView, public CoreAPI::IProfileCallback
	{
	public:
		PerformanceStatsView(Resources& resources, const HalleyAPI& api);
		~PerformanceStatsView() override;

		void update() override;
		void paint(Painter& painter) override;

		void onProfileData(std::shared_ptr<ProfilerData> data) override;
		void setNetworkStats(NetworkSession& networkSession);

		int getNumPages() const;
		int getPage() const;
		void setPage(int page);

	private:
		class FrameData {
		public:
			int variableTime = 0;
			int fixedTime = 0;
			int renderTime = 0;
		};

		class EventHistoryData {
		public:
			EventHistoryData();
			
			void update(ProfilerEventType type, int64_t value);

			int64_t getAverage() const;
			int64_t getHighest() const;
			int64_t getLowest() const;
			int64_t getHighestEver() const;
			int64_t getLowestEver() const;
			ProfilerEventType getType() const;

		private:
			ProfilerEventType type;
			int64_t highest = 0;
			int64_t lowest = std::numeric_limits<int64_t>::max();
			int64_t lastHighest = 0;
			int64_t lastLowest = 0;
			int64_t highestEver = 0;
			int64_t lowestEver = std::numeric_limits<int64_t>::max();
			AveragingLatched<int64_t> average;
		};
		
		TextRenderer headerText;
		TextRenderer fpsLabel;
		TextRenderer graphLabel;
		TextRenderer connLabel;
		Vector<TextRenderer> systemLabels;

		AveragingLatched<int64_t> totalFrameTime;
		AveragingLatched<int64_t> updateTime;
		AveragingLatched<int64_t> renderTime;
		AveragingLatched<int64_t> vsyncTime;
		AveragingLatched<int64_t> audioTime;
		
		Vector<FrameData> frameData;
		size_t lastFrameData = 0;
		HashMap<String, EventHistoryData> eventHistory;
		std::shared_ptr<ProfilerData> lastProfileData;

		bool capturing = true;
		int page = 0;

		INetworkServiceStatsListener* networkStats = nullptr;
		const NetworkSession* networkSession;

		const Sprite boxBg;
		const Sprite whitebox;


		void drawHeader(Painter& painter, bool simple);
		void drawTimeline(Painter& painter, Rect4f rect);
		void drawTimeGraph(Painter& painter, Rect4f rect);
		void drawTimeGraphThreads(Painter& painter, Rect4f rect, Range<ProfilerData::TimePoint> timeRange);
		void drawTimeGraphThread(Painter& painter, Rect4f rect, const ProfilerData::ThreadInfo& threadInfo, Range<ProfilerData::TimePoint> timeRange);
		void drawTopSystems(Painter& painter, Rect4f rect);
		void drawNetworkStats(Painter& painter, Rect4f rect);
		
		Colour4f getEventColour(ProfilerEventType event) const;
		Colour4f getNetworkStatsCol(const AckUnreliableConnectionStats::PacketStats& stats) const;

		int64_t getTimeNs(TimeLine timeline, const ProfilerData& data);
	};
}
