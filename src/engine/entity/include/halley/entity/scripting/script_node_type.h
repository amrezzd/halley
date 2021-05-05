#pragma once
#include "../entity.h"
#include "script_graph.h"
#include "script_state.h"
#include "halley/core/graphics/text/text_renderer.h"
#include "halley/time/halleytime.h"

namespace Halley {
	class World;
	class ScriptEnvironment;
	class ScriptGraph;
    class ScriptState;

	enum class ScriptNodeExecutionState {
		Done,
		Executing,
		Restart,
		Terminate
	};

	enum class ScriptNodeClassification {
		Terminator, // As in start/end, not as in Arnie
		FlowControl,
		Condition,
		Action
	};

	enum class ScriptNodeElementType : uint8_t {
		Node,
		Input,
		Output,
		Target
	};
	
	class IScriptNodeType {
	public:		
        struct Result {
	        Time timeElapsed = 0;
        	ScriptNodeExecutionState state = ScriptNodeExecutionState::Done;
        };

		struct SettingType {
			String name;
			String type;
			std::vector<String> defaultValue;
		};

		virtual ~IScriptNodeType() = default;

		virtual String getId() const = 0;
		virtual String getName() const = 0;

		virtual std::vector<SettingType> getSettingTypes() const;
		virtual std::pair<String, std::vector<ColourOverride>> getDescription(const ScriptGraphNode& node, const World& world, ScriptNodeElementType elementType, uint8_t elementIdx) const;
		virtual std::pair<String, std::vector<ColourOverride>> getNodeDescription(const ScriptGraphNode& node, const World& world) const;
		virtual std::pair<String, std::vector<ColourOverride>> getIOPinDescription(const ScriptGraphNode& node, ScriptNodeElementType elementType, uint8_t elementIdx) const;
		virtual std::pair<String, std::vector<ColourOverride>> getTargetPinDescription(const ScriptGraphNode& node, const World& world, uint8_t elementIdx) const;
		virtual String getIconName() const = 0;
		virtual ScriptNodeClassification getClassification() const = 0;
		
		virtual uint8_t getNumInputPins() const { return 1; }
		virtual uint8_t getNumOutputPins() const { return 1; }
		virtual uint8_t getNumTargetPins() const { return 0; }
        virtual bool canAdd() const { return true; }
        virtual bool canDelete() const { return true; }
		
		virtual Result update(ScriptEnvironment& environment, Time time, const ScriptGraphNode& node, IScriptStateData* curData) const = 0;
		virtual std::unique_ptr<IScriptStateData> makeData() const { return {}; }
        virtual void initData(IScriptStateData& data, const ScriptGraphNode& node) const {}
	};

	template <typename DataType, typename EnvironmentType = ScriptEnvironment>
	class ScriptNodeTypeBase : public IScriptNodeType {
	public:
		static_assert(std::is_base_of_v<IScriptStateData, DataType>);
		static_assert(std::is_base_of_v<ScriptEnvironment, EnvironmentType>);
		
		virtual Result doUpdate(EnvironmentType& environment, Time time, const ScriptGraphNode& node, DataType& curData) const = 0;
		virtual void doInitData(DataType& data, const ScriptGraphNode& node) const = 0;
		
		Result update(ScriptEnvironment& environment, Time time, const ScriptGraphNode& node, IScriptStateData* curData) const final override { return doUpdate(dynamic_cast<EnvironmentType&>(environment), time, node, *dynamic_cast<DataType*>(curData)); }
		std::unique_ptr<IScriptStateData> makeData() const override { return std::make_unique<DataType>(); }
		virtual void initData(IScriptStateData& data, const ScriptGraphNode& node) const { doInitData(dynamic_cast<DataType&>(data), node); }
	};

	template <typename EnvironmentType>
	class ScriptNodeTypeBase<void, EnvironmentType> : public IScriptNodeType {
	public:
		static_assert(std::is_base_of_v<ScriptEnvironment, EnvironmentType>);
		
		virtual Result doUpdate(EnvironmentType& environment, Time time, const ScriptGraphNode& node) const = 0;
		
		Result update(ScriptEnvironment& environment, Time time, const ScriptGraphNode& node, IScriptStateData*) const final override { return doUpdate(dynamic_cast<EnvironmentType&>(environment), time, node); }
	};

	class ScriptNodeTypeCollection {
	public:
		ScriptNodeTypeCollection();
    	void addScriptNode(std::unique_ptr<IScriptNodeType> nodeType);

		const IScriptNodeType* tryGetNodeType(const String& typeId) const;
		std::vector<String> getTypes(bool includeNonAddable) const;
		std::vector<String> getNames(bool includeNonAddable) const;

	private:
    	std::map<String, std::unique_ptr<IScriptNodeType>> nodeTypes;

		void addBasicScriptNodes();
	};
}