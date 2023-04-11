#include <cmCommandLineArgument.h>
#include <vector>
#include <string>
#include <cmMessenger.h>
#include <cmListFileCache.h>

class clint
{
    public:
        void SetSourceDirectory(const std::string & path)
        {
            m_sourceDirectory = path;
        }

        void Run()
        {
            cmListFile file;
            cmMessenger messenger;
            cmListFileBacktrace backtrace;

            if(!file.ParseFile(m_sourceDirectory.c_str(), &messenger, backtrace))
            {
                cmSystemTools::Error("Failed to parse file");
            }

            for(auto function : file.Functions)
            {
                cmSystemTools::Message(function.OriginalName());
            }
            

        }
    private:
        std::string m_sourceDirectory{"."};
};

using CommandArgument =
  cmCommandLineArgument<bool(std::string const& value, clint* state)>;




int main(int argc, char ** argv)
{
      auto EmptyStringArgLambda = [](std::string const&, clint* state) -> bool {
            return true;
    };

    auto SourceArgLambda = [](std::string const& value, clint* state){
        if (value.empty()) {
            cmSystemTools::Error("No source directory specified for -S");
            return false;
        }
        std::string path = cmSystemTools::CollapseFullPath(value);
        cmSystemTools::ConvertToUnixSlashes(path);

        state->SetSourceDirectory(path);
        return true;
    };

    std::vector<CommandArgument> arguments = {
          CommandArgument{ "", CommandArgument::Values::Zero, EmptyStringArgLambda },
          CommandArgument{ "-S", "No source directory specified for -S",
                     CommandArgument::Values::One,
                     CommandArgument::RequiresSeparator::No, SourceArgLambda }
    };

    std::vector<std::string> args(argv + 1, argv + argc);
    clint clint;

    for (decltype(args.size()) i =0; i < args.size(); i++) {
        for (auto const& m : arguments) {
        if (m.matches(args[i])) {
            const bool parsedCorrectly = m.parse(args[i], i, args, &clint);
            if (!parsedCorrectly) {
                
                return false;
            }
        }
    }    
  }
  
    clint.Run();
}