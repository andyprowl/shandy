#include <SHAndyLib/Formatting.h>
#include <SHAndyLib/SHA256.h>

#include <iostream>

int main()
{
    std::cout << "Type your message: ";

    auto message = std::string{};

    std::getline(std::cin, message);

    const auto hash = shandy::calculateSHA256(message);

    std::cout << "SHA-256: " << shandy::formatHex(hash);
}
