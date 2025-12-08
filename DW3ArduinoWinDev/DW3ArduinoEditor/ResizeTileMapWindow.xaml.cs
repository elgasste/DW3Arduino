using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;

namespace DW3ArduinoEditor
{
   public partial class ResizeTileMapWindow : Window
   {
      private uint _oldTilesX;
      private uint _oldTilesY;

      public uint NewTilesX { get; private set; } = 0;
      public uint NewTilesY { get; private set; } = 0;

      public ResizeTileMapWindow( uint oldTilesX, uint oldTilesY )
      {
         InitializeComponent();

         _oldTilesX = oldTilesX;
         _oldTilesY = oldTilesY;

         HorizontalTilesTextBox.Text = oldTilesX.ToString();
         VerticalTilesTextBox.Text = oldTilesY.ToString();
      }

      [GeneratedRegex( "[^0-9]+" )]
      private static partial Regex NonNumericRegX();

      private void TileCount_PreviewTextInput( object sender, TextCompositionEventArgs e )
      {
         var regex = NonNumericRegX();
         e.Handled = regex.IsMatch( e.Text );
      }

      private void CancelButton_Click( object sender, RoutedEventArgs e )
      {
         DialogResult = false;
         Close();
      }

      private void ResizeButton_Click( object sender, RoutedEventArgs e )
      {
         if ( !int.TryParse( HorizontalTilesTextBox.Text, out int tilesX ) )
         {
            MessageBox.Show( "Invalid value entered for horizontal tiles." );
            return;
         }
         else if ( tilesX < Constants.TileMapMinTilesX || tilesX > Constants.TileMapMaxTilesX )
         {
            MessageBox.Show( string.Format( "Horizontal tiles must be between {0} and {1}.", Constants.TileMapMinTilesX, Constants.TileMapMaxTilesX ) );
            return;
         }
         else if ( !int.TryParse( VerticalTilesTextBox.Text, out int tilesY ) )
         {
            MessageBox.Show( "Invalid value entered for vertical tiles." );
            return;
         }
         else if ( tilesY < Constants.TileMapMinTilesY || tilesY > Constants.TileMapMaxTilesY )
         {
            MessageBox.Show( string.Format( "Vertical tiles must be between {0} and {1}.", Constants.TileMapMinTilesY, Constants.TileMapMaxTilesY ) );
            return;
         }
         else if ( tilesX < _oldTilesX || tilesY < _oldTilesY )
         {
            if ( MessageBox.Show( "Warning: reducing the width or height of the map will delete some existing tile data. Are you sure?", "Please Confirm", MessageBoxButton.YesNo, MessageBoxImage.Warning ) == MessageBoxResult.Yes )
            {

            }
         }

         NewTilesX = uint.Parse( HorizontalTilesTextBox.Text );
         NewTilesY = uint.Parse( VerticalTilesTextBox.Text );
         DialogResult = true;
         Close();
      }
   }
}
