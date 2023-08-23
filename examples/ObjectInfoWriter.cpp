#include <System/MemoryManagement.h>
#include <Essentials/ArgumentParser.h>
#include <Graphics/Object3D.h>
#include <Codecs/Scene3DLoader.h>
#include <gum-maths.h>

std::vector<std::string> vFiles;

int main(int argc, char** argv)
{
    Gum::ArgumentParser* pArgumentParser = new Gum::ArgumentParser();

    pArgumentParser->usageSetup("Usage: ObjectInfoWriter [OBJFILE] [OUTFILE]", "", "--help", "-h", 20);
    if(!pArgumentParser->passArguments(argc, argv, [](std::string arg) { vFiles.push_back(arg); }))
    {
        delete pArgumentParser;
        return 1;
    }
    delete pArgumentParser;

    if(vFiles.size() != 2)
    {
        std::cerr << "Please specify exactly 2 files" << std::endl;
        return 1;
    }
    
	Mesh* mainMesh = new Mesh();
	Scene3DLoader loader;
    loader.iterateMeshes([mainMesh](unsigned int currentMesh, unsigned int numMeshes, Mesh* mesh, Bone* rootbone, std::vector<Bone*> bones) {
		mainMesh->addMesh(mesh);
		Gum::_delete(mesh);
    });
    loader.load(vFiles[0]);

    mainMesh->writeMeshInfoToFile(vFiles[1]);

	return 0;
};