<script type="text/javascript">

function popUp(number) {
    ID.innerHTML = number;
    ID.style.left = event.x + 10;
    ID.style.top	= event.y - 5;
    ID.style.visibility = "visible";
}

function hidePop() {
    ID.style.visibility = "hidden";
}

// contextMenu.js
$(document).ready(function() {
  $(body).contextMenu(
    { menu: 'OnRightClickHtmlWindow', inSpeed : 150, outSpeed : 75 },
    function(action, el, pos) {
      //クリック時の処理
    }
  );
});

</script>
