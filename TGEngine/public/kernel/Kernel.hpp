// Author: Mario
// Created: 29.12.2020
// CPPSTD: 20

#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <string_view>

namespace tge::kernel
{
	// TODO?
	class Runtime {};
	class SharedStorage {};
	
	class ISubsystem
	{
		friend class Kernel;
	
	public:
		ISubsystem(const ISubsystem&) = delete;
		ISubsystem(ISubsystem&&) = delete;
		auto operator=(const ISubsystem&)->ISubsystem & = delete;
		auto operator=(ISubsystem&&)->ISubsystem & = delete;
		virtual ~ISubsystem() = default;

		inline auto name() const noexcept -> std::string_view
		{
			return this->name_;
		}
		
		inline auto flags() const noexcept -> decltype(auto)
		{
			return this->flags_;
		}
	
	protected:
		explicit ISubsystem(const std::string_view _name, bool _init = false, bool _tick = false, bool _shutdown = false) noexcept;

		[[nodiscard]]
		inline virtual auto init([[maybe_unused]] Runtime&, [[maybe_unused]] SharedStorage&) -> bool { return true; }
		
		[[nodiscard]]
		inline virtual auto tick([[maybe_unused]] Runtime&, [[maybe_unused]] SharedStorage&) -> bool { return true; }
		
		inline virtual void shutdown([[maybe_unused]] Runtime&, [[maybe_unused]] SharedStorage&) {}

	private:
		mutable struct
		{
			bool init : 1;
			bool tick : 1;
			bool shutdown : 1;
		} flags_ = {};
		std::string_view name_;
	};
	
	class Kernel final
	{
	public:
		Kernel(Kernel&&) = delete;
		Kernel(const Kernel&) = delete;
		auto operator=(const Kernel&)->Kernel & = delete;
		auto operator=(Kernel&&)->Kernel & = delete;
		~Kernel() = default;

		[[nodiscard]]
		auto initialize(std::optional<std::vector<std::unique_ptr<ISubsystem>>&&> _systems = std::nullopt) const -> bool;
		
		[[nodiscard]]
		auto run() const -> std::uint_fast32_t;
	
	private:
		Kernel() noexcept;

		void shutdown() const;
		
		[[nodiscard]]
		auto tick() const -> bool;
		
		class Pimpl;
		std::unique_ptr<Pimpl> sys_;
	};
}