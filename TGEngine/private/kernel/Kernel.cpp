// Author: Mario
// Created: 29.12.2020
// CPPSTD: 20

#include "../../public/kernel/Kernel.hpp"

namespace tge::kernel
{
	enum class Stage : std::uint_fast8_t
	{
		OFFLINE,
		ONLINE,
		RUNNING
	};
	
	class Kernel::Pimpl final
	{
		friend Kernel;
		
		std::vector<std::unique_ptr<ISubsystem>> systems_;
		Stage stage_ = Stage::OFFLINE;
		Runtime runtime_ = {};
		SharedStorage shared_storage_ = {};
		std::uint_fast32_t tick_ = 0;
	};

	ISubsystem::ISubsystem(const std::string_view _name, bool _init, bool _tick, bool _shutdown) noexcept : name_(_name)
	{
		this->flags_.init = _init;
		this->flags_.tick = _tick;
		this->flags_.shutdown = _shutdown;
	}

	auto Kernel::initialize(std::optional<std::vector<std::unique_ptr<ISubsystem>>&&> _systems) const -> bool
	{
		if (this->sys_->stage_ != Stage::OFFLINE)
		{
			return false;
		}
		if (_systems) [[likely]] {
			this->sys_->systems_ = std::move(*_systems);
		}
		this->sys_->systems_.shrink_to_fit();
		for(auto& sys : this->sys_->systems_)
		{
			if (sys->flags().init && !sys->init(this->sys_->runtime_, this->sys_->shared_storage_)) [[unlikely]] {
				return false;
			}
		}
		this->sys_->stage_ = Stage::ONLINE;
		return true;
	}
	
	auto Kernel::run() const -> std::uint_fast32_t
	{
		if(this->sys_->stage_ != Stage::ONLINE)
		{
			return false;
		}
		this->sys_->stage_ = Stage::RUNNING;
		while (tick()) [[likely]];
		this->sys_->stage_ = Stage::ONLINE;
		return this->sys_->tick_;
	}

	Kernel::Kernel() noexcept : sys_(std::make_unique<Pimpl>()) {}

	void Kernel::shutdown() const
	{
		if (this->sys_->stage_ != Stage::ONLINE)
		{
			return;
		}
		for (auto& sys : this->sys_->systems_)
		{
			if (sys->flags().shutdown) [[likely]] {
				sys->shutdown(this->sys_->runtime_, this->sys_->shared_storage_);
			}
		}
		this->sys_->stage_ = Stage::OFFLINE;
	}
	
	auto Kernel::tick() const -> bool
	{
		++this->sys_->tick_;
		for (auto& sys : this->sys_->systems_)
		{
			if (sys->flags().tick && !sys->tick(this->sys_->runtime_, this->sys_->shared_storage_)) [[unlikely]] {
				return false;
			}
		}
		return true;
	}
}
