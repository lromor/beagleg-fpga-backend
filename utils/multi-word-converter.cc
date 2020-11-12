#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <sstream>
#include <cctype>
#include <map>

typedef std::function<std::vector<std::string>(std::string)> FromCallback;
typedef std::function<std::string(const std::vector<std::string> &)> ToCallback;

template <typename Iter>
std::string join(Iter begin, Iter end, std::string const& separator) {
  std::ostringstream result;
  if (begin != end)
    result << *begin++;
  while (begin != end)
    result << separator << *begin++;
  return result.str();
}

static std::vector<std::string> dash_case_input(std::string src) {
  std::transform(src.begin(), src.end(), src.begin(), tolower);
  std::stringstream ss(src);
  std::string tmp;
  std::vector<std::string> tokens;

  while(std::getline(ss, tmp, '-'))
    tokens.push_back(tmp);

  return tokens;
}

static std::string pascal_case_output(const std::vector<std::string> &tokens) {
  std::ostringstream ss;
  for (auto s : tokens) {
    s[0] = std::toupper(s[0]);
    ss << s;
  }
  return ss.str();
}

static std::string snake_case_output(const std::vector<std::string> &tokens) {
  return join(tokens.begin(), tokens.end(), "_");
}

static const std::map<std::string, std::tuple<std::string, FromCallback>> from_ops = {
  {"dash", {"foo-bar", dash_case_input}},
};

static const std::map<std::string, std::tuple<std::string, ToCallback>> to_ops = {
  {"pascal", {"FooBar", pascal_case_output}},
  {"snake", {"foo_bar", snake_case_output}}
};

static int usage(const char *prog) {
  fprintf(stderr, "Usage: %s <from> <to> <string>\n", prog);
  return 1;
}


int main(int argc, char *argv[]) {
  if (argc != 4) return usage(argv[0]);
  const std::string from_style(argv[1]);
  const std::string to_style(argv[2]);
  const std::string target(argv[3]);

  if (from_ops.find(from_style) == from_ops.end()) {
    fprintf(stderr, "Style <from>:\"%s\" is not supported.\n", from_style.c_str());
    return usage(argv[0]);
  }

  const std::vector<std::string> tokens = std::get<1>(from_ops.at(from_style))(target);

  if (to_ops.find(to_style) == to_ops.end()) {
    fprintf(stderr, "To style \"%s\" is not supported.", to_style.c_str());
    return usage(argv[0]);
  }

  std::cout << std::get<1>(to_ops.at(to_style))(tokens) << std::endl;
  return 0;
}
