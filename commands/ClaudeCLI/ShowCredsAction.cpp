#include "ShowCredsAction.hpp"

#include <Security/Security.h>

std::string DefaultCredsReader() {
  CFMutableDictionaryRef query = CFDictionaryCreateMutable(
      nullptr, 0,
      &kCFTypeDictionaryKeyCallBacks,
      &kCFTypeDictionaryValueCallBacks);

  CFDictionarySetValue(query, kSecClass,       kSecClassGenericPassword);
  CFDictionarySetValue(query, kSecAttrService, CFSTR("Claude Code-credentials"));
  CFDictionarySetValue(query, kSecReturnData,  kCFBooleanTrue);

  CFTypeRef result = nullptr;
  const OSStatus status = SecItemCopyMatching(query, &result);
  CFRelease(query);

  if (status == errSecItemNotFound)
    throw std::runtime_error("Claude Code-credentials not found in keychain");
  if (status != errSecSuccess)
    throw std::runtime_error("Keychain error: " + std::to_string(status));

  const auto data = static_cast<CFDataRef>(result);
  std::string password(
      reinterpret_cast<const char *>(CFDataGetBytePtr(data)),
      static_cast<std::size_t>(CFDataGetLength(data)));
  CFRelease(result);
  return password;
}
