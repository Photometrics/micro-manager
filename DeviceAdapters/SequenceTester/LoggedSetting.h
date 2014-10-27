// Mock device adapter for testing of device sequencing
//
// Copyright (C) 2014 University of California, San Francisco.
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
// for more details.
//
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//
// Author: Mark Tsuchida

#pragma once

#include "DeviceBase.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <string>


// A "setting" in this device adapter is a property or a property-like entity
// (e.g. camera exposure, stage position).
template <class TDevice>
class LoggedSetting
{
   SettingLogger* logger_;
   TDevice* device_;
   const std::string name_;

protected:
   SettingLogger* GetLogger() { return logger_; }
   const SettingLogger* GetLogger() const { return logger_; }
   TDevice* GetDevice() { return device_; }
   const TDevice* GetDevice() const { return device_; }
   std::string GetName() const { return name_; }

public:
   LoggedSetting(SettingLogger* logger, TDevice* device,
         const std::string& name);

   void MarkDeviceBusy() { logger_->MarkBusy(device_->GetDeviceName()); }
};


template <class TDevice>
class BoolSetting : private LoggedSetting<TDevice>
{
   typedef BoolSetting<TDevice> Self;
   typedef LoggedSetting<TDevice> Super;

public:
   typedef boost::shared_ptr<Self> Ptr;

   BoolSetting(SettingLogger* logger, TDevice* device,
         const std::string& name, bool initialValue);

   static Ptr New(SettingLogger* logger, TDevice* device,
         const std::string& name, bool initialValue)
   { return boost::make_shared<Self>(logger, device, name, initialValue); }

   int Set(bool newValue);
   int Get(bool& value) const;
   bool Get() const;

   enum PropertyDisplay
   {
      ON_OFF,
      YES_NO,
      ONE_ZERO, // Not nice, but used e.g. for shutter state
   };
   MM::ActionFunctor* NewPropertyAction(PropertyDisplay displayMode);
};


template <class TDevice>
class IntegerSetting : private LoggedSetting<TDevice>
{
   bool hasMinMax_;
   long min_;
   long max_;

   typedef IntegerSetting<TDevice> Self;
   typedef LoggedSetting<TDevice> Super;

public:
   typedef boost::shared_ptr<Self> Ptr;

   IntegerSetting(SettingLogger* logger, TDevice* device,
         const std::string& name, long initialValue,
         bool hasMinMax, long minimum, long maximum);

   static Ptr New(SettingLogger* logger, TDevice* device,
         const std::string& name, long initialValue,
         bool hasMinMax, long minimum = 0, long maximum = 0)
   {
      return boost::make_shared<Self>(logger, device, name, initialValue,
         hasMinMax, minimum, maximum);
   }

   bool HasMinMax() const { return hasMinMax_; }
   long GetMin() const { return min_; }
   long GetMax() const { return max_; }

   int Set(long newValue);
   int Get(long& value) const;
   long Get() const;
   MM::ActionFunctor* NewPropertyAction();
};


template <class TDevice>
class FloatSetting : private LoggedSetting<TDevice>
{
   bool hasMinMax_;
   double min_;
   double max_;

   typedef FloatSetting<TDevice> Self;
   typedef LoggedSetting<TDevice> Super;

public:
   typedef boost::shared_ptr<Self> Ptr;

   FloatSetting(SettingLogger* logger, TDevice* device,
         const std::string& name, double initialValue,
         bool hasMinMax, double minimum, double maximum);

   static Ptr New(SettingLogger* logger, TDevice* device,
         const std::string& name, double initialValue,
         bool hasMinMax, double minimum = 0.0, double maximum = 0.0)
   {
      return boost::make_shared<Self>(logger, device, name, initialValue,
            hasMinMax, minimum, maximum);
   }


   bool HasMinMax() const { return hasMinMax_; }
   double GetMin() const { return min_; }
   double GetMax() const { return max_; }

   int Set(double newValue);
   int Get(double& value) const;
   double Get() const;
   MM::ActionFunctor* NewPropertyAction();
};


template <class TDevice>
class OneShotSetting : private LoggedSetting<TDevice>
{
   typedef OneShotSetting<TDevice> Self;
   typedef LoggedSetting<TDevice> Super;

public:
   typedef boost::shared_ptr<Self> Ptr;

   OneShotSetting(SettingLogger* logger, TDevice* device,
         const std::string& name);

   static Ptr New(SettingLogger* logger, TDevice* device,
         const std::string& name)
   { return boost::make_shared<Self>(logger, device, name); }


   int Set();
};