bool nextgen;

void MessageHandler(F4SE::MessagingInterface::Message* a_message)
{
	switch (a_message->type) {
	case F4SE::MessagingInterface::kPostLoad:
		break;
	case F4SE::MessagingInterface::kGameDataReady:
		{	
			auto& trampoline = F4SE::GetTrampoline();	
			if (!nextgen) {
				REL::Relocation<std::uintptr_t> target1{ REL::ID(22388), 0x52};
				REL::Relocation<std::uintptr_t> target2{ REL::ID(22388), 0x703};
												
				trampoline.write_branch<6>(target1.address(), target2.address());
			}
			else {
				REL::Relocation<std::uintptr_t> target1{ REL::ID(2195766), 0x4F};
				REL::Relocation<std::uintptr_t> target2{ REL::ID(2195766), 0x6DD};
						
				trampoline.write_branch<6>(target1.address(), target2.address());
			}
		}
		break;
	default:
		break;
	}
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_F4SE, F4SE::PluginInfo* a_info)
{
	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = Plugin::NAME.data();
	a_info->version = Plugin::Version::PROJECT_VERSION_MAJOR;

	return true;
}

DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_F4SE)
{
#ifndef NDEBUG
	MessageBoxA(NULL, "Loaded. You can now attach the debugger or continue execution.", Plugin::NAME.data(), NULL);
#endif
	auto ver = a_F4SE->RuntimeVersion();
	
	F4SE::Init(a_F4SE);
	if (ver >= F4SE::RUNTIME_1_10_980) {
		nextgen = true;
	}
	DKUtil::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
	INFO("{} v{} loaded."sv, Plugin::NAME, Plugin::Version);
	
	F4SE::AllocTrampoline(1 << 7);
	
	const auto messaging = F4SE::GetMessagingInterface();
	messaging->RegisterListener(MessageHandler);

	return true;
}
