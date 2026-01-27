using DW3ArduinoEditor.Graphics;
using DW3ArduinoEditor.SaveData;
using System.Collections.ObjectModel;

namespace DW3ArduinoEditor.ViewModels
{
   public class TextureSetViewModel : ViewModelBase
   {
      private readonly ITextureImageProvider _textureImageProvider;

      private uint _index;
      public uint Index
      {
         get => _index;
         set
         {
            SetProperty( ref _index, value );

            TextureImages = [];
            foreach ( var index in TexturePoolIndexes )
            {
               if ( _textureImageProvider is not null )
               {
                  TextureImages.Add( new( _textureImageProvider.GetImageFromIndex( index ), Constants.EditorTileSize, Constants.EditorTileSize ) );
               }
            }
         }
      }

      private string _name = string.Empty;
      public string Name
      {
         get => _name;
         set => SetProperty( ref _name, value );
      }

      public ObservableCollection<uint> TexturePoolIndexes { get; private set; } = [];
      public ObservableCollection<TextureImageViewModel> TextureImages { get; private set; } = [];

      public int WaterTextureIndex { get; private set; } = -1;
      public int HorizontalBridgeTextureIndex { get; private set; } = -1;
      public int VerticalBridgeTextureIndex { get; private set; } = -1;
      public int ShoalTextureIndex { get; private set; } = -1;
      public uint ShoreTextureStartIndex { get; private set; }

      public TextureSetViewModel( ITextureImageProvider textureImageProvider, uint index, string name, int waterTextureIndex, uint shoreTextureStartIndex )
      {
         _textureImageProvider = textureImageProvider;
         Index = index;
         Name = name;
         WaterTextureIndex = waterTextureIndex;
         ShoreTextureStartIndex = shoreTextureStartIndex;
      }

      public TextureSetViewModel( ITextureImageProvider textureImageProvider, TextureSetSaveData saveData )
      {
         _textureImageProvider = textureImageProvider;
         Name = saveData.Name;
         WaterTextureIndex = saveData.WaterTextureIndex;
         HorizontalBridgeTextureIndex = saveData.HorizontalBridgeTextureIndex;
         VerticalBridgeTextureIndex = saveData.VerticalBridgeTextureIndex;
         ShoalTextureIndex = saveData.ShoalTextureIndex;
         ShoreTextureStartIndex= saveData.ShoreTextureStartIndex;

         foreach ( var index in saveData.TexturePoolIndexes )
         {
            TexturePoolIndexes.Add( index );
         }

         Index = saveData.Index;
      }
   }
}
