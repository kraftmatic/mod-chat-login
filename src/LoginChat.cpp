/**
    This plugin can be used for common player customizations
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Configuration/Config.h"
#include "ChannelMgr.h"
#include "Chat.h"

class LoginChat : public PlayerScript{
public:

    LoginChat() : PlayerScript("LoginChat") { }

    void OnLogin(Player* player) override {
        if (sConfigMgr->GetBoolDefault("LoginChat.enabled", true))
        {
          std::string channelName = sConfigMgr->GetStringDefault("LoginChat.name", "world");
          QueryResult result = CharacterDatabase.PQuery("SELECT channelId password FROM Channels WHERE name = '%s'", channelName.c_str());
          if (!result)
            return;
          uint32 channelId = (*result)[0].GetUInt32();
          std::string password = (*result)[1].GetString();

          if (ChannelMgr* cMgr = ChannelMgr::forTeam(player->GetTeamId()))
          {
            if (Channel* channel = cMgr->GetJoinChannel(channelName, channelId))
              channel->JoinChannel(player, password);
          }
        }
    }
};

class LoginChat_Config : public WorldScript
{
public:
    LoginChat_Config() : WorldScript("LoginChat_Config") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            std::string conf_path = _CONF_DIR;
            std::string cfg_file = conf_path + "/LoginChat.conf";
            std::string cfg_def_file = cfg_file + ".dist";

            sConfigMgr->LoadMore(cfg_def_file.c_str());
            sConfigMgr->LoadMore(cfg_file.c_str());
        }
    }
};

void AddLoginChatScripts() {
    new LoginChat;
    new LoginChat_Config;
}
