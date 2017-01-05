//#line 2 "/opt/ros/kinetic/share/dynamic_reconfigure/templates/ConfigType.h.template"
// *********************************************************
// 
// File autogenerated for the bitbots_motion package 
// by the dynamic_reconfigure package.
// Please do not edit.
// 
// ********************************************************/

#ifndef __bitbots_motion__MOTION_PARAMSCONFIG_H__
#define __bitbots_motion__MOTION_PARAMSCONFIG_H__

#include <dynamic_reconfigure/config_tools.h>
#include <limits>
#include <ros/node_handle.h>
#include <dynamic_reconfigure/ConfigDescription.h>
#include <dynamic_reconfigure/ParamDescription.h>
#include <dynamic_reconfigure/Group.h>
#include <dynamic_reconfigure/config_init_mutex.h>
#include <boost/any.hpp>

namespace bitbots_motion
{
  class motion_paramsConfigStatics;
  
  class motion_paramsConfig
  {
  public:
    class AbstractParamDescription : public dynamic_reconfigure::ParamDescription
    {
    public:
      AbstractParamDescription(std::string n, std::string t, uint32_t l, 
          std::string d, std::string e)
      {
        name = n;
        type = t;
        level = l;
        description = d;
        edit_method = e;
      }
      
      virtual void clamp(motion_paramsConfig &config, const motion_paramsConfig &max, const motion_paramsConfig &min) const = 0;
      virtual void calcLevel(uint32_t &level, const motion_paramsConfig &config1, const motion_paramsConfig &config2) const = 0;
      virtual void fromServer(const ros::NodeHandle &nh, motion_paramsConfig &config) const = 0;
      virtual void toServer(const ros::NodeHandle &nh, const motion_paramsConfig &config) const = 0;
      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, motion_paramsConfig &config) const = 0;
      virtual void toMessage(dynamic_reconfigure::Config &msg, const motion_paramsConfig &config) const = 0;
      virtual void getValue(const motion_paramsConfig &config, boost::any &val) const = 0;
    };

    typedef boost::shared_ptr<AbstractParamDescription> AbstractParamDescriptionPtr;
    typedef boost::shared_ptr<const AbstractParamDescription> AbstractParamDescriptionConstPtr;
    
    template <class T>
    class ParamDescription : public AbstractParamDescription
    {
    public:
      ParamDescription(std::string name, std::string type, uint32_t level, 
          std::string description, std::string edit_method, T motion_paramsConfig::* f) :
        AbstractParamDescription(name, type, level, description, edit_method),
        field(f)
      {}

      T (motion_paramsConfig::* field);

      virtual void clamp(motion_paramsConfig &config, const motion_paramsConfig &max, const motion_paramsConfig &min) const
      {
        if (config.*field > max.*field)
          config.*field = max.*field;
        
        if (config.*field < min.*field)
          config.*field = min.*field;
      }

      virtual void calcLevel(uint32_t &comb_level, const motion_paramsConfig &config1, const motion_paramsConfig &config2) const
      {
        if (config1.*field != config2.*field)
          comb_level |= level;
      }

      virtual void fromServer(const ros::NodeHandle &nh, motion_paramsConfig &config) const
      {
        nh.getParam(name, config.*field);
      }

      virtual void toServer(const ros::NodeHandle &nh, const motion_paramsConfig &config) const
      {
        nh.setParam(name, config.*field);
      }

      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, motion_paramsConfig &config) const
      {
        return dynamic_reconfigure::ConfigTools::getParameter(msg, name, config.*field);
      }

      virtual void toMessage(dynamic_reconfigure::Config &msg, const motion_paramsConfig &config) const
      {
        dynamic_reconfigure::ConfigTools::appendParameter(msg, name, config.*field);
      }

      virtual void getValue(const motion_paramsConfig &config, boost::any &val) const
      {
        val = config.*field;
      }
    };

    class AbstractGroupDescription : public dynamic_reconfigure::Group
    {
      public:
      AbstractGroupDescription(std::string n, std::string t, int p, int i, bool s)
      {
        name = n;
        type = t;
        parent = p;
        state = s;
        id = i;
      }

      std::vector<AbstractParamDescriptionConstPtr> abstract_parameters;
      bool state;

      virtual void toMessage(dynamic_reconfigure::Config &msg, const boost::any &config) const = 0;
      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, boost::any &config) const =0;
      virtual void updateParams(boost::any &cfg, motion_paramsConfig &top) const= 0;
      virtual void setInitialState(boost::any &cfg) const = 0;


      void convertParams()
      {
        for(std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = abstract_parameters.begin(); i != abstract_parameters.end(); ++i)
        {
          parameters.push_back(dynamic_reconfigure::ParamDescription(**i));
        }
      }
    };

    typedef boost::shared_ptr<AbstractGroupDescription> AbstractGroupDescriptionPtr;
    typedef boost::shared_ptr<const AbstractGroupDescription> AbstractGroupDescriptionConstPtr;

    template<class T, class PT>
    class GroupDescription : public AbstractGroupDescription
    {
    public:
      GroupDescription(std::string name, std::string type, int parent, int id, bool s, T PT::* f) : AbstractGroupDescription(name, type, parent, id, s), field(f)
      {
      }

      GroupDescription(const GroupDescription<T, PT>& g): AbstractGroupDescription(g.name, g.type, g.parent, g.id, g.state), field(g.field), groups(g.groups)
      {
        parameters = g.parameters;
        abstract_parameters = g.abstract_parameters;
      }

      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, boost::any &cfg) const
      {
        PT* config = boost::any_cast<PT*>(cfg);
        if(!dynamic_reconfigure::ConfigTools::getGroupState(msg, name, (*config).*field))
          return false;

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = &((*config).*field);
          if(!(*i)->fromMessage(msg, n))
            return false;
        }

        return true;
      }

      virtual void setInitialState(boost::any &cfg) const
      {
        PT* config = boost::any_cast<PT*>(cfg);
        T* group = &((*config).*field);
        group->state = state;

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = boost::any(&((*config).*field));
          (*i)->setInitialState(n);
        }

      }

      virtual void updateParams(boost::any &cfg, motion_paramsConfig &top) const
      {
        PT* config = boost::any_cast<PT*>(cfg);

        T* f = &((*config).*field);
        f->setParams(top, abstract_parameters);

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = &((*config).*field);
          (*i)->updateParams(n, top);
        }
      }

      virtual void toMessage(dynamic_reconfigure::Config &msg, const boost::any &cfg) const
      {
        const PT config = boost::any_cast<PT>(cfg);
        dynamic_reconfigure::ConfigTools::appendGroup<T>(msg, name, id, parent, config.*field);

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          (*i)->toMessage(msg, config.*field);
        }
      }

      T (PT::* field);
      std::vector<motion_paramsConfig::AbstractGroupDescriptionConstPtr> groups;
    };
    
class DEFAULT
{
  public:
    DEFAULT()
    {
      state = true;
      name = "Default";
    }

    void setParams(motion_paramsConfig &config, const std::vector<AbstractParamDescriptionConstPtr> params)
    {
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator _i = params.begin(); _i != params.end(); ++_i)
      {
        boost::any val;
        (*_i)->getValue(config, val);

        if("dyn_falling_active"==(*_i)->name){dyn_falling_active = boost::any_cast<bool>(val);}
        if("ground_coefficient"==(*_i)->name){ground_coefficient = boost::any_cast<double>(val);}
        if("threshold_gyro_y_front"==(*_i)->name){threshold_gyro_y_front = boost::any_cast<double>(val);}
        if("threshold_gyro_y_back"==(*_i)->name){threshold_gyro_y_back = boost::any_cast<double>(val);}
        if("threshold_gyro_x_right"==(*_i)->name){threshold_gyro_x_right = boost::any_cast<double>(val);}
        if("threshold_gyro_x_left"==(*_i)->name){threshold_gyro_x_left = boost::any_cast<double>(val);}
      }
    }

    bool dyn_falling_active;
double ground_coefficient;
double threshold_gyro_y_front;
double threshold_gyro_y_back;
double threshold_gyro_x_right;
double threshold_gyro_x_left;

    bool state;
    std::string name;

    
}groups;



//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      bool dyn_falling_active;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      double ground_coefficient;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      double threshold_gyro_y_front;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      double threshold_gyro_y_back;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      double threshold_gyro_x_right;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      double threshold_gyro_x_left;
//#line 218 "/opt/ros/kinetic/share/dynamic_reconfigure/templates/ConfigType.h.template"

    bool __fromMessage__(dynamic_reconfigure::Config &msg)
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();

      int count = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        if ((*i)->fromMessage(msg, *this))
          count++;

      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); i ++)
      {
        if ((*i)->id == 0)
        {
          boost::any n = boost::any(this);
          (*i)->updateParams(n, *this);
          (*i)->fromMessage(msg, n);
        }
      }

      if (count != dynamic_reconfigure::ConfigTools::size(msg))
      {
        ROS_ERROR("motion_paramsConfig::__fromMessage__ called with an unexpected parameter.");
        ROS_ERROR("Booleans:");
        for (unsigned int i = 0; i < msg.bools.size(); i++)
          ROS_ERROR("  %s", msg.bools[i].name.c_str());
        ROS_ERROR("Integers:");
        for (unsigned int i = 0; i < msg.ints.size(); i++)
          ROS_ERROR("  %s", msg.ints[i].name.c_str());
        ROS_ERROR("Doubles:");
        for (unsigned int i = 0; i < msg.doubles.size(); i++)
          ROS_ERROR("  %s", msg.doubles[i].name.c_str());
        ROS_ERROR("Strings:");
        for (unsigned int i = 0; i < msg.strs.size(); i++)
          ROS_ERROR("  %s", msg.strs[i].name.c_str());
        // @todo Check that there are no duplicates. Make this error more
        // explicit.
        return false;
      }
      return true;
    }

    // This version of __toMessage__ is used during initialization of
    // statics when __getParamDescriptions__ can't be called yet.
    void __toMessage__(dynamic_reconfigure::Config &msg, const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__, const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__) const
    {
      dynamic_reconfigure::ConfigTools::clear(msg);
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->toMessage(msg, *this);

      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); ++i)
      {
        if((*i)->id == 0)
        {
          (*i)->toMessage(msg, *this);
        }
      }
    }
    
    void __toMessage__(dynamic_reconfigure::Config &msg) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();
      __toMessage__(msg, __param_descriptions__, __group_descriptions__);
    }
    
    void __toServer__(const ros::NodeHandle &nh) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->toServer(nh, *this);
    }

    void __fromServer__(const ros::NodeHandle &nh)
    {
      static bool setup=false;

      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->fromServer(nh, *this);

      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();
      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); i++){
        if (!setup && (*i)->id == 0) {
          setup = true;
          boost::any n = boost::any(this);
          (*i)->setInitialState(n);
        }
      }
    }

    void __clamp__()
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const motion_paramsConfig &__max__ = __getMax__();
      const motion_paramsConfig &__min__ = __getMin__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->clamp(*this, __max__, __min__);
    }

    uint32_t __level__(const motion_paramsConfig &config) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      uint32_t level = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->calcLevel(level, config, *this);
      return level;
    }
    
    static const dynamic_reconfigure::ConfigDescription &__getDescriptionMessage__();
    static const motion_paramsConfig &__getDefault__();
    static const motion_paramsConfig &__getMax__();
    static const motion_paramsConfig &__getMin__();
    static const std::vector<AbstractParamDescriptionConstPtr> &__getParamDescriptions__();
    static const std::vector<AbstractGroupDescriptionConstPtr> &__getGroupDescriptions__();
    
  private:
    static const motion_paramsConfigStatics *__get_statics__();
  };
  
  template <> // Max and min are ignored for strings.
  inline void motion_paramsConfig::ParamDescription<std::string>::clamp(motion_paramsConfig &config, const motion_paramsConfig &max, const motion_paramsConfig &min) const
  {
    return;
  }

  class motion_paramsConfigStatics
  {
    friend class motion_paramsConfig;
    
    motion_paramsConfigStatics()
    {
motion_paramsConfig::GroupDescription<motion_paramsConfig::DEFAULT, motion_paramsConfig> Default("Default", "", 0, 0, true, &motion_paramsConfig::groups);
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.dyn_falling_active = 0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.dyn_falling_active = 1;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.dyn_falling_active = 0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<bool>("dyn_falling_active", "bool", 1, "False: Servos will be turned of when falling; true: robot tries to fall dynamically", "", &motion_paramsConfig::dyn_falling_active)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<bool>("dyn_falling_active", "bool", 1, "False: Servos will be turned of when falling; true: robot tries to fall dynamically", "", &motion_paramsConfig::dyn_falling_active)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.ground_coefficient = 0.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.ground_coefficient = 2.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.ground_coefficient = 1.25;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("ground_coefficient", "double", 1, "Hardness of the floor, 1.25 for artificial turf(soft), 1 for hard ground", "", &motion_paramsConfig::ground_coefficient)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("ground_coefficient", "double", 1, "Hardness of the floor, 1.25 for artificial turf(soft), 1 for hard ground", "", &motion_paramsConfig::ground_coefficient)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.threshold_gyro_y_front = -100.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.threshold_gyro_y_front = 0.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.threshold_gyro_y_front = -50.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("threshold_gyro_y_front", "double", 1, "Threshold harder -> earlier reaction, but more false positives.", "", &motion_paramsConfig::threshold_gyro_y_front)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("threshold_gyro_y_front", "double", 1, "Threshold harder -> earlier reaction, but more false positives.", "", &motion_paramsConfig::threshold_gyro_y_front)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.threshold_gyro_y_back = 0.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.threshold_gyro_y_back = -100.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.threshold_gyro_y_back = 50.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("threshold_gyro_y_back", "double", 1, "Threshold harder -> earlier reaction, but more false positives.", "", &motion_paramsConfig::threshold_gyro_y_back)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("threshold_gyro_y_back", "double", 1, "Threshold harder -> earlier reaction, but more false positives.", "", &motion_paramsConfig::threshold_gyro_y_back)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.threshold_gyro_x_right = -100.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.threshold_gyro_x_right = 0.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.threshold_gyro_x_right = -30.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("threshold_gyro_x_right", "double", 1, "Threshold harder -> earlier reaction, but more false positives.", "", &motion_paramsConfig::threshold_gyro_x_right)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("threshold_gyro_x_right", "double", 1, "Threshold harder -> earlier reaction, but more false positives.", "", &motion_paramsConfig::threshold_gyro_x_right)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __min__.threshold_gyro_x_left = 0.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __max__.threshold_gyro_x_left = -100.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __default__.threshold_gyro_x_left = 30.0;
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.abstract_parameters.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("threshold_gyro_x_left", "double", 1, "Threshold harder -> earlier reaction, but more false positives.", "", &motion_paramsConfig::threshold_gyro_x_left)));
//#line 259 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __param_descriptions__.push_back(motion_paramsConfig::AbstractParamDescriptionConstPtr(new motion_paramsConfig::ParamDescription<double>("threshold_gyro_x_left", "double", 1, "Threshold harder -> earlier reaction, but more false positives.", "", &motion_paramsConfig::threshold_gyro_x_left)));
//#line 233 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      Default.convertParams();
//#line 233 "/opt/ros/kinetic/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator.py"
      __group_descriptions__.push_back(motion_paramsConfig::AbstractGroupDescriptionConstPtr(new motion_paramsConfig::GroupDescription<motion_paramsConfig::DEFAULT, motion_paramsConfig>(Default)));
//#line 353 "/opt/ros/kinetic/share/dynamic_reconfigure/templates/ConfigType.h.template"

      for (std::vector<motion_paramsConfig::AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); ++i)
      {
        __description_message__.groups.push_back(**i);
      }
      __max__.__toMessage__(__description_message__.max, __param_descriptions__, __group_descriptions__); 
      __min__.__toMessage__(__description_message__.min, __param_descriptions__, __group_descriptions__); 
      __default__.__toMessage__(__description_message__.dflt, __param_descriptions__, __group_descriptions__); 
    }
    std::vector<motion_paramsConfig::AbstractParamDescriptionConstPtr> __param_descriptions__;
    std::vector<motion_paramsConfig::AbstractGroupDescriptionConstPtr> __group_descriptions__;
    motion_paramsConfig __max__;
    motion_paramsConfig __min__;
    motion_paramsConfig __default__;
    dynamic_reconfigure::ConfigDescription __description_message__;

    static const motion_paramsConfigStatics *get_instance()
    {
      // Split this off in a separate function because I know that
      // instance will get initialized the first time get_instance is
      // called, and I am guaranteeing that get_instance gets called at
      // most once.
      static motion_paramsConfigStatics instance;
      return &instance;
    }
  };

  inline const dynamic_reconfigure::ConfigDescription &motion_paramsConfig::__getDescriptionMessage__() 
  {
    return __get_statics__()->__description_message__;
  }

  inline const motion_paramsConfig &motion_paramsConfig::__getDefault__()
  {
    return __get_statics__()->__default__;
  }
  
  inline const motion_paramsConfig &motion_paramsConfig::__getMax__()
  {
    return __get_statics__()->__max__;
  }
  
  inline const motion_paramsConfig &motion_paramsConfig::__getMin__()
  {
    return __get_statics__()->__min__;
  }
  
  inline const std::vector<motion_paramsConfig::AbstractParamDescriptionConstPtr> &motion_paramsConfig::__getParamDescriptions__()
  {
    return __get_statics__()->__param_descriptions__;
  }

  inline const std::vector<motion_paramsConfig::AbstractGroupDescriptionConstPtr> &motion_paramsConfig::__getGroupDescriptions__()
  {
    return __get_statics__()->__group_descriptions__;
  }

  inline const motion_paramsConfigStatics *motion_paramsConfig::__get_statics__()
  {
    const static motion_paramsConfigStatics *statics;
  
    if (statics) // Common case
      return statics;

    boost::mutex::scoped_lock lock(dynamic_reconfigure::__init_mutex__);

    if (statics) // In case we lost a race.
      return statics;

    statics = motion_paramsConfigStatics::get_instance();
    
    return statics;
  }


}

#endif // __MOTION_PARAMSRECONFIGURATOR_H__