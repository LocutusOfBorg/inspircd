/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2008 InspIRCd Development Team
 * See: http://www.inspircd.org/wiki/index.php/Credits
 *
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

#include "inspircd.h"

/* $ModDesc: A module which logs all oper commands to the ircd log at default loglevel. */

class ModuleOperLog : public Module
{
 private:
	 
 public:
	ModuleOperLog(InspIRCd* Me) : Module(Me)
	{
		
		Implementation eventlist[] = { I_OnPreCommand, I_On005Numeric };
		ServerInstance->Modules->Attach(eventlist, this, 2);
	}
 
	virtual ~ModuleOperLog()
	{
	}
 
	virtual Version GetVersion()
	{
		return Version(1,2,0,0,VF_VENDOR,API_VERSION);
	}
 

	virtual int OnPreCommand(const std::string &command, const std::vector<std::string> &parameters, User *user, bool validated, const std::string &original_line)
	{
		/* If the command doesnt appear to be valid, we dont want to mess with it. */
		if (!validated)
			return 0;
 
		if ((IS_OPER(user)) && (IS_LOCAL(user)) && (user->HasPermission(command)))
		{
			Command* thiscommand = ServerInstance->Parser->GetHandler(command);
			if ((thiscommand) && (thiscommand->flags_needed == 'o'))
			{
				std::string plist;
				for (int j = 0; j < (int)parameters.size(); j++)
					plist.append(std::string(" ")+parameters[j]);

				ServerInstance->Logs->Log("m_operlog",DEFAULT,"OPERLOG: [%s!%s@%s] %s%s",user->nick.c_str(), user->ident.c_str(), user->host, command.c_str(), plist.c_str());
			} 
		}

		return 0;
	}

	virtual void On005Numeric(std::string &output)
	{
		output.append(" OPERLOG");
	}

};
 
 
MODULE_INIT(ModuleOperLog)
