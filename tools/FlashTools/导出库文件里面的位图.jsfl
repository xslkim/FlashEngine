var folderPath = './'; //fl.browseForFolderURL("保存文件的路径")
var folderPath = fl.getDocumentDOM().pathURI
fl.trace(folderPath)

folderPath = folderPath.substring(0, folderPath.lastIndexOf("/"))
fl.trace(folderPath)

folderPath = folderPath + '/LIBRARY'
fl.trace(folderPath)

fl.trace(' ');
fl.trace(' ');
fl.trace(' ');
fl.trace(' ');
fl.trace(' ');
fl.trace('start');

var lib = fl.getDocumentDOM().library;

var items = lib.items;

for(var i =0; i < items.length; i++)
{
	if(items[i].itemType == "bitmap")
	{
		var itemType = items[i].itemType;
		var itemName = items[i].name;
		fl.trace('index:' + i + ' ' + itemName + '   type:' + itemType);
		//items[i].exportToFile(folderPath + "/"+ itemName.substring(itemName.lastIndexOf("/") + 1, itemName.length));
		//items[i].exportToFile(folderPath + "/" + items[i].name);
		if(items[i].name.indexOf('.png') != -1 || items[i].name.indexOf('.PNG') != -1)
		{
		   items[i].exportToFile(folderPath + "/" + items[i].name);
		}
		else if(items[i].name.indexOf('.jpg') != -1 || items[i].name.indexOf('.JPG') != -1 || items[i].name.indexOf('.jpeg') != -1 || items[i].name.indexOf('.JPEG') != -1)
		{
		   items[i].exportToFile(folderPath + "/" + items[i].name);
		}
		else
		{
		   items[i].exportToFile(folderPath + "/" + items[i].name + ".png");
		}
	}
}
fl.trace('export finished')
