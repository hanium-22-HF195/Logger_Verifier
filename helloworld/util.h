
void XOR(char *dst, const char *src)
{
    const uint32_t MAGIC = 0x5f;

    // iterate over all chars in 'input', xor it with the magic number into dst.
    int i = 0;
    // Request for Share  ^ PubKey(Logger)
    for (; src[i]; i++)
    {
        dst[i] = src[i] ^ MAGIC;
    }
    // Redundant if dst is large enough, but be sure we add a terminating null
    // byte.
    dst[i] = '\0';
}

const char *SHARE_FILE = "shares.key";
int write_share(std::set<grpc::string> shareSet)
{
    grpc::string sharesPath(SHARE_FILE);

    // write File
    std::ofstream writeFile(sharesPath.data());
    if (writeFile.is_open())
    {
        for (auto it = shareSet.begin(); it != shareSet.end(); it++)
        {
            std::cout << "value: " << *it << std::endl;
            writeFile << *it << "\n";
        }
    }
    writeFile.close();
    return 1;
}

std::string read_share()
{

    std::string sharesPath(SHARE_FILE);
    std::string shareString("");

    std::ifstream openFile(sharesPath.data());
    if (openFile.is_open())
    {
        std::string line;
        while (getline(openFile, line))
        {
            shareString.append(line + '\n');
        }
        openFile.close();
    }
    return shareString;
}